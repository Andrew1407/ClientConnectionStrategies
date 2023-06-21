// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Clients/ClientLabels.h"
#include "RequestFormWidget.generated.h"

class UTextBlock;
class UButton;
class UEditableTextBox;
class UComboBoxString;

class AGameModeBase;
class FRegexPattern;

class IConnection;
class IClient;

class UClientCollection;
class UDefaultFieldsFilling;
/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API URequestFormWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	#pragma region ELEMENTS

	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UButton> SwitchBtn;

	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultTextBlock;
	
	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UButton> ConnectBtn;

	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UButton> DisconnectBtn;

	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UButton> SendBtn;

	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UEditableTextBox> HostField;

	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UEditableTextBox> PortField;

	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UComboBoxString> ConnectionTypeSelector;

	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UEditableTextBox> FirstParamField;

	UPROPERTY(BlueprintReadOnly, Category="RequestFormWidget|UI", meta = (BindWidget))
	TObjectPtr<UEditableTextBox> SecondParamField;

	#pragma endregion

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RequestFormWidget|Assets", meta = (AllowPrivateAccess = true))
	TObjectPtr<UClientCollection> ClientCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RequestFormWidget|Assets", meta = (AllowPrivateAccess = true))
	TObjectPtr<UDefaultFieldsFilling> DefaultFieldsFilling;

	#pragma region EVENT_HANDLERS

	UFUNCTION()
	void HideWidget();

	UFUNCTION()
	void ConnectOnClick();

	UFUNCTION()
	void DisconnectOnClick();

	UFUNCTION()
	void SendOnClick();

	UFUNCTION()
	void OnTypeSelect(FString SelectedItem, ESelectInfo::Type SelectionType);

	#pragma endregion

	#pragma region CONNECTION_HANDLING

	UFUNCTION()
	void OnResponse(const struct FResponseData& ResponseData, bool bSuccess);

	UFUNCTION()
	void OnConnect(EClientLabels Type, bool bSuccess);

	UFUNCTION()
	void OnDisconnect(EClientLabels Type, bool bSuccess);

	#pragma endregion

	AGameModeBase* GetGameMode();
	void SetSelectedClient(AGameModeBase* GameMode, EClientLabels SelectedType, const FString SelectedOption);
	bool SetClientAddress(UObject* Client);
};
