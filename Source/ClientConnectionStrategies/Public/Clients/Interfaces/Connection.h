// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Clients/ClientLabels.h"
#include "Connection.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FConnectionDelegate, EClientLabels, Type, bool, bSuccess);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConnection : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLIENTCONNECTIONSTRATEGIES_API IConnection
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IConnection")
	bool Connected();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IConnection")
	void Connect(const FConnectionDelegate& ConnectionDelegate);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IConnection")
	void Disconnect(const FConnectionDelegate& DisconnectionDelegate);
};
