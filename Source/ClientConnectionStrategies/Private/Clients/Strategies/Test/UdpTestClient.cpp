// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/Strategies/Test/UdpTestClient.h"
#include "Clients/ClientLabels.h"

void UUdpTestClient::SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate)
{
    OnResponse = ResponseDelegate;
}

void UUdpTestClient::Send_Implementation(const FRequestData& RequestData)
{
    OnResponse.ExecuteIfBound(ResponseData, ResponseStatus);
}

void UUdpTestClient::SetAddress_Implementation(const FString& Host, const int32& Port)
{
    HostParam = Host;
    PortParam = Port;
}

void UUdpTestClient::Connect_Implementation(const FConnectionDelegate& ConnectionDelegate)
{
    ConnectionDelegate.ExecuteIfBound(EClientLabels::UDP, ConnectioneStatus);
	ConnectioneState = true;
}

void UUdpTestClient::Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate)
{
    DisconnectionDelegate.ExecuteIfBound(EClientLabels::UDP, ConnectioneStatus);
	ConnectioneState = false;
}
