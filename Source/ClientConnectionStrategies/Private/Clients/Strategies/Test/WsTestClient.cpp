// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/Strategies/Test/WsTestClient.h"
#include "Clients/ClientLabels.h"

void UWsTestClient::SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate)
{
    OnResponse = ResponseDelegate;
}

void UWsTestClient::Send_Implementation(const FRequestData& RequestData)
{
    OnResponse.ExecuteIfBound(ResponseData, ResponseStatus);
}

void UWsTestClient::SetAddress_Implementation(const FString& Host, const int32& Port)
{
    HostParam = Host;
    PortParam = Port;
}

void UWsTestClient::Connect_Implementation(const FConnectionDelegate& ConnectionDelegate)
{
    ConnectionDelegate.ExecuteIfBound(EClientLabels::WS, ConnectioneStatus);
	ConnectioneState = true;
}

void UWsTestClient::Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate)
{
    DisconnectionDelegate.ExecuteIfBound(EClientLabels::WS, ConnectioneStatus);
	ConnectioneState = false;
}
