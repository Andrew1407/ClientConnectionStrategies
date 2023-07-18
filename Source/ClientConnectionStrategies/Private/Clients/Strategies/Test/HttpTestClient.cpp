// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/Strategies/Test/HttpTestClient.h"

void UHttpTestClient::SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate)
{
    OnResponse = ResponseDelegate;
}

void UHttpTestClient::Send_Implementation(const FRequestData& RequestData)
{
    OnResponse.ExecuteIfBound(ResponseData, ResponseStatus);
}

void UHttpTestClient::SetAddress_Implementation(const FString& Host, const int32& Port)
{
    HostParam = Host;
    PortParam = Port;
}
