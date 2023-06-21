// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClientFormSwitch.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UClientFormSwitch : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, Category="ClientFormSwitch|UI", meta = (BindWidget))
	TObjectPtr<UButton> ShowFormBtn;

private:
	UFUNCTION()
	void ShowClientForm();
};
