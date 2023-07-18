// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/Strategies/Test/TcpTestClient.h"
#include "Clients/ClientLabels.h"

void UTcpTestClient::SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate)
{
    OnResponse = ResponseDelegate;
}

void UTcpTestClient::Send_Implementation(const FRequestData& RequestData)
{
    OnResponse.ExecuteIfBound(ResponseData, ResponseStatus);
}

void UTcpTestClient::SetAddress_Implementation(const FString& Host, const int32& Port)
{
    HostParam = Host;
    PortParam = Port;
}

void UTcpTestClient::Connect_Implementation(const FConnectionDelegate& ConnectionDelegate)
{
    ConnectionDelegate.ExecuteIfBound(EClientLabels::TCP, ConnectioneStatus);
	ConnectioneState = true;
}

void UTcpTestClient::Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate)
{
    DisconnectionDelegate.ExecuteIfBound(EClientLabels::TCP, ConnectioneStatus);
	ConnectioneState = false;
}
