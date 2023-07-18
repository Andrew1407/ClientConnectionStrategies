// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Clients/Strategies/UdpClient.h"
#include "Clients/DataContainers/ResponseData.h"
#include "UdpTestClient.generated.h"

/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UUdpTestClient : public UUdpClient
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual void SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate) override;
	FORCEINLINE virtual void Send_Implementation(const FRequestData& RequestData) override;
	virtual void SetAddress_Implementation(const FString& Host, const int32& Port) override;

	FORCEINLINE virtual bool Connected_Implementation() override { return ConnectioneState; }
	virtual void Connect_Implementation(const FConnectionDelegate& ConnectionDelegate) override;
	virtual void Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	bool ResponseStatus = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	bool ConnectioneState = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	bool ConnectioneStatus = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	bool DisonnectioneStatus = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	FResponseDeledate OnResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	FResponseData ResponseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	FString HostParam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	int32 PortParam;
};
