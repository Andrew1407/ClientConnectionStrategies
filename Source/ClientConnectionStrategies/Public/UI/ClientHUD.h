// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/ClientFormViewer.h"
#include "ClientHUD.generated.h"

class UClientFormSwitch;
class URequestFormWidget;
/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API AClientHUD : public AHUD, public IClientFormViewer
{
	GENERATED_BODY()

public:
	virtual void ShowForm_Implementation();
	virtual void HideForm_Implementation();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadWrite, Category = "PichHUD", meta = (AllowPrivateAccess = true))
	TObjectPtr<URequestFormWidget> ClientRequestWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PichHUD", meta = (AllowPrivateAccess = true))
	TSubclassOf<URequestFormWidget> ClientWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "PichHUD", meta = (AllowPrivateAccess = true))
	TObjectPtr<UClientFormSwitch> ClientFormSwitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PichHUD", meta = (AllowPrivateAccess = true))
	TSubclassOf<UClientFormSwitch> ClientFormSwitchClass;

	void CreateClientWidget();
	void CreateSwitchWidget();
};
