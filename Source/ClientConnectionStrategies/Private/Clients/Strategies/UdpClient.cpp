// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/Strategies/UdpClient.h"
#include "Clients/DataContainers/RequestData.h"
#include "Clients/DataContainers/ResponseData.h"

#include "Clients/Strategies/Utils/ClientOptions.h"
#include "Clients/Strategies/Utils/ConnectionType.h"
#include "Clients/Strategies/Utils/ConnectionUtils.h"

#include "JsonObjectConverter.h"
#include "JsonUtilities.h"

#include "Async/Async.h"
#include "Networking.h"

const FConnectionType UUdpClient::ConnectionType { NAME_DGram, TEXT("UDPClientSocket") };

UUdpClient::UUdpClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    ClientOptions.ResponseTimeout = 3;
}

void UUdpClient::BeginDestroy()
{
    ConnectionUtils::ResetSocket(Socket);
    Super::BeginDestroy();
}

void UUdpClient::SetAddress_Implementation(const FString& Host, const int32& Port)
{
    ClientOptions.Host = Host;
    ClientOptions.Port = Port;
}

void UUdpClient::SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate)
{
    ClientOptions.ResponseDeledate = ResponseDelegate;
}

bool UUdpClient::Connected_Implementation()
{
    return ConnectionUtils::SocketConnected(Socket);
}

void UUdpClient::Send_Implementation(const FRequestData& RequestData)
{
    const TFunctionRef<void()> OnFailure = [&] { ClientOptions.ResponseDeledate.ExecuteIfBound({}, false); };
    if (!Execute_Connected(this)) return OnFailure();
    TArray<uint8> MessageArray;
    FString Content;
    bool IsParsed = FJsonObjectConverter::UStructToJsonObjectString(RequestData, Content);
    if (!(IsParsed && ConnectionUtils::SendData(Socket, Content))) return OnFailure();
    HandleReceivedData(ENamedThreads::GameThread, [&] (const FString& ReceivedMessage)
    {
        FResponseData ResponseData;
        bool IsSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FResponseData>(ReceivedMessage, &ResponseData, 0, 0);
        ClientOptions.ResponseDeledate.ExecuteIfBound(ResponseData, IsSuccess);
    });
}

void UUdpClient::Connect_Implementation(const FConnectionDelegate& ConnectionDelegate)
{
    const TFunctionRef<void()> OnFailure = [&] { ConnectionDelegate.ExecuteIfBound(EClientLabels::UDP, false); };
    if (Execute_Connected(this)) return OnFailure();
    ConnectionUtils::ResetSocket(Socket);
    bool Connected = false;
    Socket = ConnectionUtils::MakeSocketConnection(ConnectionType.Name, ConnectionType.Description, ClientOptions.Host, ClientOptions.Port, Connected);
    if (!Connected) return OnFailure();
    if (!ConnectionUtils::SendData(Socket, TEXT("heartbeat"))) return OnFailure();
    HandleReceivedData(ENamedThreads::GameThread, [&, ConnectionDelegate] (const FString& ReceivedMessage)
    {
        bool bHealthcheck = ReceivedMessage == TEXT("OK");
        if (!bHealthcheck) ConnectionUtils::ResetSocket(Socket);
        ConnectionDelegate.ExecuteIfBound(EClientLabels::UDP, bHealthcheck);
    });
}

void UUdpClient::Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate)
{
    bool IsConnected = Execute_Connected(this);
    if (IsConnected) ConnectionUtils::ResetSocket(Socket);
    DisconnectionDelegate.ExecuteIfBound(EClientLabels::UDP, IsConnected);
}

void UUdpClient::HandleReceivedData(ENamedThreads::Type Context, TFunction<void(const FString&)> Callback)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&, Context, Callback = MoveTemp(Callback)]
    {
        int32 BufferSize = 1 << 14;
        FString Message = ConnectionUtils::ReceiveData(Socket, ClientOptions.ResponseTimeout, BufferSize);
        AsyncTask(Context, [Message = MoveTemp(Message), Callback] { Callback(Message); });
    });
}
