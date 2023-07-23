// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/Strategies/WsClient.h"
#include "Clients/DataContainers/RequestData.h"
#include "Clients/DataContainers/ResponseData.h"
#include "Clients/Strategies/Utils/ClientOptions.h"

#include "JsonObjectConverter.h"
#include "JsonUtilities.h"

#include "WebSocketsModule.h" 
#include "IWebSocket.h"       

UWsClient::UWsClient(const FObjectInitializer& ObjectInitializer)
{
    ClientOptions.ResponseTimeout = 3;
}

void UWsClient::BeginDestroy()
{
    if (Socket.IsValid()) Socket->Close();
    Super::BeginDestroy();
}

void UWsClient::SetAddress_Implementation(const FString& Host, const int32& Port)
{
    ClientOptions.Host = Host;
    ClientOptions.Port = Port;
}

void UWsClient::SetResponseDelegate_Implementation(const FResponseDelegate& ResponseDelegate)
{
    ClientOptions.ResponseDelegate = ResponseDelegate;
}

void UWsClient::Send_Implementation(const FRequestData& RequestData)
{
    const TFunctionRef<void()> OnFailure = [&] { ClientOptions.ResponseDelegate.ExecuteIfBound({}, false); };
    if (!Execute_Connected(this)) return OnFailure();
    FString Content;
    bool IsParsed = FJsonObjectConverter::UStructToJsonObjectString(RequestData, Content);
    return IsParsed ? Socket->Send(Content) : OnFailure();
}

bool UWsClient::Connected_Implementation()
{
    return Socket.IsValid() && Socket->IsConnected();
}

void UWsClient::Connect_Implementation(const FConnectionDelegate& ConnectionDelegate)
{
    const TFunction<void(bool)> OnConnect = [ConnectionDelegate] (bool bState) { ConnectionDelegate.ExecuteIfBound(EClientLabels::WS, bState); };
    if (Execute_Connected(this)) return OnConnect(false);
    const FString Url = FString::Printf(TEXT("ws://%s:%d/"), *ClientOptions.Host, ClientOptions.Port);
    Socket = FWebSocketsModule::Get().CreateWebSocket(Url);
    ConnectionTimer = FTimerHandle();
    
    const TFunction<void(bool)> HandleTimer = [&, OnConnect] (bool bState)
    {
        GetWorld()->GetTimerManager().ClearTimer(ConnectionTimer);
        OnConnect(bState);
    };
    Socket->OnConnected().AddLambda([HandleTimer] { HandleTimer(true); });
    Socket->OnConnectionError().AddLambda([HandleTimer] (const FString & Error) { HandleTimer(false); });
    Socket->OnClosed().AddLambda([&] (int32 StatusCode, const FString& Reason, bool bWasClean) { TriggerDisconnect(); });
    Socket->OnMessage().AddLambda([&] (const FString& Message)
    {
        FResponseData ResponseData;
        bool IsSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FResponseData>(Message, &ResponseData, 0, 0);
        ClientOptions.ResponseDelegate.ExecuteIfBound(ResponseData, IsSuccess);
    });
    Socket->Connect();
    GetWorld()->GetTimerManager().SetTimer(ConnectionTimer, [OnConnect] { OnConnect(false); }, ClientOptions.ResponseTimeout, false);
}

void UWsClient::Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate)
{
    if (!Execute_Connected(this))
        return (void)DisconnectionDelegate.ExecuteIfBound(EClientLabels::WS, false);
    OnDisconnectDelegate = DisconnectionDelegate;
    Socket->Close();
    Socket.Reset();
}
