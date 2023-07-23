// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Clients/Strategies/UdpClient.h"
#include "Clients/DataContainers/ResponseData.h"
#include "Clients/Strategies/Test/ClientTestData.h"
#include "UdpTestClient.generated.h"

/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UUdpTestClient : public UUdpClient, public ConnectionTestData
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual void SetResponseDelegate_Implementation(const FResponseDelegate& ResponseDelegate) override { OnResponse = ResponseDelegate; }
	FORCEINLINE virtual void Send_Implementation(const FRequestData& RequestData) override { OnResponse.ExecuteIfBound(ResponseData, ResponseStatus); }
	virtual void SetAddress_Implementation(const FString& Host, const int32& Port) override
	{
		HostParam = Host;
		PortParam = Port;
	}

	FORCEINLINE virtual bool Connected_Implementation() override { return ConnectionState; }
	virtual void Connect_Implementation(const FConnectionDelegate& ConnectionDelegate) override
	{
		ConnectionDelegate.ExecuteIfBound(EClientLabels::UDP, ConnectionStatus);
		ConnectionState = true;
	}
	virtual void Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate) override
	{
		DisconnectionDelegate.ExecuteIfBound(EClientLabels::UDP, ConnectionStatus);
		ConnectionState = false;
	}
};
