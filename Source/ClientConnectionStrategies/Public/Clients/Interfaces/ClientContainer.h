// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Clients/ClientLabels.h"
#include "ClientContainer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UClientContainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLIENTCONNECTIONSTRATEGIES_API IClientContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IClientContainer")
	bool SetClient(EClientLabels Type);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IClientContainer")
	EClientLabels GetClientType();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IClientContainer")
	UObject* GetClient();
};
