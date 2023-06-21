// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Client.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FResponseDeledate, const struct FResponseData&, ResponseData, bool, bSuccess);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UClient : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLIENTCONNECTIONSTRATEGIES_API IClient
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IClient")
	void SetAddress(const FString& Host, const int32& Port);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IClient")
	void SetResponseDelegate(const FResponseDeledate& ResponseDelegate);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IClient")
	void Send(const struct FRequestData& RequestData);
};
