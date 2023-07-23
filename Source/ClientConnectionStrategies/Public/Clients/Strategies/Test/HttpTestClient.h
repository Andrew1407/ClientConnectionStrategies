// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Clients/Strategies/HttpClient.h"
#include "Clients/DataContainers/ResponseData.h"
#include "Clients/Strategies/Test/ClientTestData.h"
#include "HttpTestClient.generated.h"

/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UHttpTestClient : public UHttpClient, public ClientTestData
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
};
