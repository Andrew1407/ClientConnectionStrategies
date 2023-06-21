// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RequestFormWidget.h"

#include "GameFramework/HUD.h"
#include "UI/ClientFormViewer.h"
#include "UI/ClientFormUtils.h"

#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Internationalization/Regex.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"

#include "Clients/DataContainers/RequestData.h"
#include "Clients/DataContainers/ResponseData.h"

#include "Clients/Interfaces/ClientContainer.h"
#include "Clients/Interfaces/Client.h"
#include "Clients/Interfaces/Connection.h"

#include "DataAssets/ClientCollection.h"
#include "DataAssets/DefaultFieldsFilling.h"

void URequestFormWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (IsValid(SwitchBtn)) SwitchBtn->OnClicked.AddDynamic(this, &URequestFormWidget::HideWidget);
    if (IsValid(ConnectBtn)) ConnectBtn->OnClicked.AddDynamic(this, &URequestFormWidget::ConnectOnClick);
    if (IsValid(DisconnectBtn)) DisconnectBtn->OnClicked.AddDynamic(this, &URequestFormWidget::DisconnectOnClick);
    if (IsValid(SendBtn)) SendBtn->OnClicked.AddDynamic(this, &URequestFormWidget::SendOnClick);

    bool IsCollectionValid = ClientCollection && ClientCollection->IsValidLowLevel();
    bool IsFillingValid = DefaultFieldsFilling && DefaultFieldsFilling->IsValidLowLevel();

    if (IsValid(ConnectionTypeSelector))
    {
        ConnectionTypeSelector->OnSelectionChanged.AddDynamic(this, &URequestFormWidget::OnTypeSelect);
        if (IsCollectionValid)
            for (const auto& Pair : ClientCollection->ClientNames) ConnectionTypeSelector->AddOption(Pair.Key.ToString());
        else
            ConnectionTypeSelector->AddOption("NONE");
        ConnectionTypeSelector->SetSelectedIndex(0);
    }

    const TMap<UEditableTextBox*, TPair<FString, TFunctionRef<FString()>>> FieldParams {
        {FirstParamField, MakeTuple("Denominations", [&] { return DefaultFieldsFilling->DenominationsStr(); }) },
        {SecondParamField, MakeTuple("Amount", [&] { return DefaultFieldsFilling->AmountStr(); }) },
        {HostField, MakeTuple("Host", [&] { return DefaultFieldsFilling->Host; }) },
        {PortField, MakeTuple("Port", [&] { return DefaultFieldsFilling->PortStr(); }) },
    };

    for (const auto& Pair : FieldParams)
    {
        if (!IsValid(Pair.Key)) continue;
        Pair.Key->SetHintText(FText::FromString(Pair.Value.Key));
        if (IsFillingValid) Pair.Key->SetText(FText::FromString(Pair.Value.Value()));
    }

    if (IsValid(ResultTextBlock))
    {
        ResultTextBlock->SetText(FText::FromString("Result:"));
        const FColor ResultColor = IsCollectionValid ? ClientCollection->DefaultColor : FColor::White;
        ResultTextBlock->SetColorAndOpacity(ResultColor);
    }
}

void URequestFormWidget::ConnectOnClick() 
{
    UClientFormUtils::HandleConnectionAction(GetGameMode(), ConnectBtn, [&] (UObject* Client)
    {
        bool IsStateValid = !IConnection::Execute_Connected(Client) && SetClientAddress(Client);
        if (!IsStateValid) return false;
        FConnectionDelegate ConnectionDelegate;
        ConnectionDelegate.BindDynamic(this, &URequestFormWidget::OnConnect);
        IConnection::Execute_Connect(Client, ConnectionDelegate);
        return true;
    });
}

void URequestFormWidget::DisconnectOnClick()
{
    UClientFormUtils::HandleConnectionAction(GetGameMode(), DisconnectBtn, [&] (UObject* Client)
    {
        bool IsStateValid = IConnection::Execute_Connected(Client);
        if (!IsStateValid) return false;
        FConnectionDelegate DisconnectionDelegate;
        DisconnectionDelegate.BindDynamic(this, &URequestFormWidget::OnDisconnect);
        IConnection::Execute_Disconnect(Client, DisconnectionDelegate);
        return true;
    });
}

void URequestFormWidget::SendOnClick()
{
    AGameModeBase* GameMode = GetGameMode();
    if (!IsValid(GameMode)) return;
    FRequestData RequestData;
    bool IsParamsValid = UClientFormUtils::TryParseParamsFields(FirstParamField.Get(), SecondParamField.Get(), RequestData.Denominations, RequestData.Amount);
    if (!IsParamsValid) return;
    UObject* Client = IClientContainer::Execute_GetClient(GameMode);
    bool IsClient = UKismetSystemLibrary::DoesImplementInterface(Client, UClient::StaticClass());
    if (!IsClient) return;
    bool bCanSend = true;
    bool IsConnection = UKismetSystemLibrary::DoesImplementInterface(Client, UConnection::StaticClass());
    if (!IsConnection) bCanSend = SetClientAddress(Client);
    if (!bCanSend) return;
    SendBtn->SetIsEnabled(false);
    IClient::Execute_Send(Client, RequestData);
}

void URequestFormWidget::HideWidget()
{
    AHUD* CurrentHUD = GetOwningPlayer()->GetHUD();
    bool IsViewer = UKismetSystemLibrary::DoesImplementInterface(CurrentHUD, UClientFormViewer::StaticClass());
    if (!(IsValid(CurrentHUD) || IsViewer)) return;
    AGameModeBase* GameMode = GetGameMode();
    if (!IsValid(GameMode)) return;
    UObject* Client = IClientContainer::Execute_GetClient(GameMode);
    bool IsConnection = UKismetSystemLibrary::DoesImplementInterface(Client, UConnection::StaticClass());
    if (IsConnection && IConnection::Execute_Connected(Client))
    {
        FConnectionDelegate DisconnectionDelegate;
        DisconnectionDelegate.BindDynamic(this, &URequestFormWidget::OnDisconnect);
        IConnection::Execute_Disconnect(Client, DisconnectionDelegate);
    }
    IClientFormViewer::Execute_HideForm(CurrentHUD);
}

void URequestFormWidget::OnResponse(const FResponseData& ResponseData, bool bSuccess)
{
    bool IsCollectionValid = ClientCollection && ClientCollection->IsValidLowLevel();
    if (!bSuccess)
    {
        ResultTextBlock->SetText(FText::FromString("Result: NaN"));
        const FColor ErrorColor = IsCollectionValid ? ClientCollection->ErrorColor : FColor::Red;
        ResultTextBlock->SetColorAndOpacity(ErrorColor);
        return SendBtn->SetIsEnabled(true);
    }
    AGameModeBase* GameMode = GetGameMode();
    EClientLabels CurrectType = IsValid(GameMode) ? IClientContainer::Execute_GetClientType(GameMode) : EClientLabels::NONE;
    bool IsColorAvailable = IsCollectionValid && ClientCollection->ClientColors.Contains(CurrectType);
    FColor ResultColor;
    if (IsColorAvailable)
        ResultColor = *ClientCollection->ClientColors.Find(CurrectType);
    else
        ResultColor = IsCollectionValid ? ClientCollection->DefaultColor : FColor::White;
    const FString ResultText = FString::Printf(TEXT("Result: %d"), ResponseData.Result);
    ResultTextBlock->SetText(FText::FromString(ResultText));
    ResultTextBlock->SetColorAndOpacity(ResultColor);
    SendBtn->SetIsEnabled(true);
}

void URequestFormWidget::OnConnect(EClientLabels Type, bool bSuccess)
{
    if (!bSuccess) return ConnectBtn->SetIsEnabled(true);
    DisconnectBtn->SetIsEnabled(true);
    SendBtn->SetIsEnabled(true);
}

void URequestFormWidget::OnDisconnect(EClientLabels Type, bool bSuccess)
{
    bool IsCollectionValid = ClientCollection && ClientCollection->IsValidLowLevel();
    AGameModeBase* GameMode = GetGameMode();
    if (!(IsValid(GameMode) || IsCollectionValid)) return;
    const FString SelectedOption = ConnectionTypeSelector->GetSelectedOption();
    const EClientLabels* LabelToSelectPtr = ClientCollection->ClientNames.Find(*SelectedOption);
    if (!LabelToSelectPtr) return;
    if (*LabelToSelectPtr != Type) return SetSelectedClient(GameMode, *LabelToSelectPtr, SelectedOption);
    SendBtn->SetIsEnabled(!bSuccess);
    DisconnectBtn->SetIsEnabled(!bSuccess);
    ConnectBtn->SetIsEnabled(true);
}

void URequestFormWidget::OnTypeSelect(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    bool IsCollectionValid = ClientCollection && ClientCollection->IsValidLowLevel();
    if (!IsCollectionValid) return;
    const EClientLabels* LabelToSelectPtr = ClientCollection->ClientNames.Find(*SelectedItem);
    AGameModeBase* GameMode = GetGameMode();
    bool IsDataValid = LabelToSelectPtr && IsValid(GameMode);
    if (!IsDataValid) return;
    for (const auto& Button : { SendBtn, ConnectBtn, DisconnectBtn })
        Button->SetVisibility(ESlateVisibility::Collapsed);
    UObject* PreviousClient = IClientContainer::Execute_GetClient(GameMode);
    bool IsPrevConnection = UKismetSystemLibrary::DoesImplementInterface(PreviousClient, UConnection::StaticClass());
    const TFunctionRef<void()> SetSelected = [&] { SetSelectedClient(GameMode, *LabelToSelectPtr, SelectedItem); };
    if (!IsPrevConnection) return SetSelected();
    FConnectionDelegate DisconnectionDelegate;
    DisconnectionDelegate.BindDynamic(this, &URequestFormWidget::OnDisconnect);
    bool IsConnected = IConnection::Execute_Connected(PreviousClient);
    if (!IsConnected) return SetSelected();
    IConnection::Execute_Disconnect(PreviousClient, DisconnectionDelegate);
}

AGameModeBase* URequestFormWidget::GetGameMode()
{
    AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
    bool IsClientContainer = UKismetSystemLibrary::DoesImplementInterface(GameMode, UClientContainer::StaticClass());
    return IsClientContainer ? GameMode : nullptr;
}

void URequestFormWidget::SetSelectedClient(AGameModeBase* GameMode, EClientLabels SelectedType, const FString SelectedOption)
{
    FResponseDeledate ResponseDeledate;
    ResponseDeledate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(URequestFormWidget, OnResponse));
    const TArray<UButton*> Buttons { SendBtn.Get(), ConnectBtn.Get(), DisconnectBtn.Get() };
    UClientFormUtils::SetSelectedClient(Buttons, GameMode, SelectedType, SelectedOption, ResponseDeledate);
}

bool URequestFormWidget::SetClientAddress(UObject* Client)
{
    FString Host;
    int32 Port;
    bool IsSuccess = UClientFormUtils::TryParseAddressFields(HostField.Get(), PortField.Get(), Host, Port);
    if (IsSuccess) IClient::Execute_SetAddress(Client, Host, Port);
    return IsSuccess;
}
