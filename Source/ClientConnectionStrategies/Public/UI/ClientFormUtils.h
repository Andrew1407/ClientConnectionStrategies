// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Clients/ClientLabels.h"
#include "Clients/Interfaces/Client.h"
#include "ClientFormUtils.generated.h"

class UEditableTextBox;
class AGameModeBase;
class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UClientFormUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="ClientFormUtils")
	static bool ValidateFields(const TMap<UEditableTextBox*, FString>& FieldValidators);

	UFUNCTION(BlueprintCallable, Category="ClientFormUtils")
	static void SetSelectedClient(const TArray<UButton*>& Buttons, AGameModeBase* GameMode,
    	EClientLabels SelectedType, const FString& SelectedOption, const FResponseDeledate& ResponseDeledate);

	UFUNCTION(BlueprintCallable, Category="ClientFormUtils")
	static bool TryParseAddressFields(UEditableTextBox* HostField, UEditableTextBox* PortField, FString& Host, int32& Port);
	
	UFUNCTION(BlueprintCallable, Category="ClientFormUtils")
	static bool TryParseParamsFields(UEditableTextBox* FirstParamField, UEditableTextBox* SecondParamField, TArray<int32>& Denominations, int32& Amount);

	static void HandleConnectionAction(AGameModeBase* GameMode, const TObjectPtr<UButton>& Button, TFunctionRef<bool(UObject*)> ConnectionCheck);
};
