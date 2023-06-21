// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/Strategies/TcpClient.h"
#include "Clients/DataContainers/RequestData.h"
#include "Clients/DataContainers/ResponseData.h"

#include "Clients/Strategies/Utils/ClientOptions.h"
#include "Clients/Strategies/Utils/ConnectionType.h"
#include "Clients/Strategies/Utils/ConnectionUtils.h"

#include "JsonObjectConverter.h"
#include "JsonUtilities.h"

#include "Async/Async.h"
#include "Networking.h"

const FConnectionType UTcpClient::ConnectionType { NAME_Stream, TEXT("TCPClientSocket") };

UTcpClient::UTcpClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    ClientOptions.ResponseTimeout = 3;
}

void UTcpClient::BeginDestroy()
{
    ConnectionUtils::ResetSocket(Socket);
    Super::BeginDestroy();
}

void UTcpClient::SetAddress_Implementation(const FString& Host, const int32& Port)
{
    ClientOptions.Host = Host;
    ClientOptions.Port = Port;
}

void UTcpClient::SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate)
{
    ClientOptions.ResponseDeledate = ResponseDelegate;
}

bool UTcpClient::Connected_Implementation()
{
    return ConnectionUtils::SocketConnected(Socket);
}

void UTcpClient::Send_Implementation(const FRequestData& RequestData)
{
    const TFunctionRef<void()> OnFailure = [&] { ClientOptions.ResponseDeledate.ExecuteIfBound({}, false); };
    if (!Execute_Connected(this)) return OnFailure();
    TArray<uint8> MessageArray;
    FString Content;
    bool IsParsed = FJsonObjectConverter::UStructToJsonObjectString(RequestData, Content);
    if (!(IsParsed && ConnectionUtils::SendData(Socket, Content))) return OnFailure();
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&]
    {
        int32 BufferSize = 1 << 14;
        FString Message = ConnectionUtils::ReceiveData(Socket, ClientOptions.ResponseTimeout, BufferSize);
        AsyncTask(ENamedThreads::GameThread, [&, Message = MoveTemp(Message)]
        {
            FResponseData ResponseData;
            bool IsSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FResponseData>(Message, &ResponseData, 0, 0);
            ClientOptions.ResponseDeledate.ExecuteIfBound(ResponseData, IsSuccess);
        });
    });
}

void UTcpClient::Connect_Implementation(const FConnectionDelegate& ConnectionDelegate)
{
    if (Execute_Connected(this)) return (void)ConnectionDelegate.ExecuteIfBound(EClientLabels::TCP, false);
    ConnectionUtils::ResetSocket(Socket);
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&, ConnectionDelegate]
    {
        bool Connected = false;
        Socket = ConnectionUtils::MakeSocketConnection(ConnectionType.Name, ConnectionType.Description, ClientOptions.Host, ClientOptions.Port, Connected);
        AsyncTask(ENamedThreads::GameThread, [&, Connected, ConnectionDelegate]
        {
            ConnectionDelegate.ExecuteIfBound(EClientLabels::TCP, Connected);
        });
    });
}

void UTcpClient::Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate)
{
    bool IsConnected = Execute_Connected(this);
    if (IsConnected) ConnectionUtils::ResetSocket(Socket);
    DisconnectionDelegate.ExecuteIfBound(EClientLabels::TCP, IsConnected);
}
