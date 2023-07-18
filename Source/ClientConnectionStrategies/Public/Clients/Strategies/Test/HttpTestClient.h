// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Clients/Strategies/HttpClient.h"
#include "Clients/DataContainers/ResponseData.h"
#include "HttpTestClient.generated.h"

/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UHttpTestClient : public UHttpClient
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual void SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate) override;
	FORCEINLINE virtual void Send_Implementation(const FRequestData& RequestData) override;
	virtual void SetAddress_Implementation(const FString& Host, const int32& Port) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	bool ResponseStatus = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	FResponseDeledate OnResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	FResponseData ResponseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	FString HostParam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	int32 PortParam;
};
