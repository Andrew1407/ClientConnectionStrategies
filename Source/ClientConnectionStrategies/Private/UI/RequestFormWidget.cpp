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
        ConnectionTypeSelector->AddOption(TEXT("NONE"));
        if (IsCollectionValid)
        {
            for (const auto& Pair : ClientCollection->ClientNames)
            {
                if (Pair.Value != EClientLabels::NONE)
                    ConnectionTypeSelector->AddOption(Pair.Key.ToString());
            }
        }
        ConnectionTypeSelector->SetSelectedIndex(0);
    }

    if (IsValid(FirstParamField))
    {
        FirstParamField->SetHintText(FText::FromString(TEXT("Denominations")));
        if (IsFillingValid) FirstParamField->SetText(FText::FromString(DefaultFieldsFilling->DenominationsStr()));
    }

    if (IsValid(SecondParamField))
    {
        SecondParamField->SetHintText(FText::FromString(TEXT("Amount")));
        if (IsFillingValid) SecondParamField->SetText(FText::FromString(DefaultFieldsFilling->AmountStr()));
    }

    if (IsValid(HostField))
    {
        HostField->SetHintText(FText::FromString(TEXT("Host")));
        if (IsFillingValid) HostField->SetText(FText::FromString(DefaultFieldsFilling->Host));
        const FString Host = UClientFormUtils::GetHostCmdArgument();
        if (!Host.IsEmpty()) HostField->SetText(FText::FromString(Host));
    }

    if (IsValid(PortField))
    {
        PortField->SetHintText(FText::FromString(TEXT("Port")));
        if (IsFillingValid) PortField->SetText(FText::FromString(DefaultFieldsFilling->PortStr()));
        const int32 Port = UClientFormUtils::GetPortCmdArgument();
        if (Port > 0) PortField->SetText(FText::FromString(FString::FromInt(Port)));
    }

    if (IsValid(ResultTextBlock))
    {
        ResultTextBlock->SetText(FText::FromString(TEXT("Result:")));
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
        ResultTextBlock->SetText(FText::FromString(TEXT("Result: NaN")));
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
    AGameModeBase* GameMode = GetGameMode();
    if (!(IsCollectionValid && IsValid(GameMode))) return;
    for (auto& Button : { SendBtn, ConnectBtn, DisconnectBtn }) Button->SetVisibility(ESlateVisibility::Collapsed);
    UObject* PreviousClient = IClientContainer::Execute_GetClient(GameMode);
    bool IsPrevConnection = UKismetSystemLibrary::DoesImplementInterface(PreviousClient, UConnection::StaticClass());
    const EClientLabels* LabelToSelectPtr = ClientCollection->ClientNames.Find(*SelectedItem);
    EClientLabels LabelToSelect = LabelToSelectPtr ? *LabelToSelectPtr : EClientLabels::NONE;
    const TFunctionRef<void()> SetSelected = [this, GameMode, LabelToSelect, SelectedItem = MoveTemp(SelectedItem)]
    {
        SetSelectedClient(GameMode, LabelToSelect, SelectedItem);
    };
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
    FResponseDelegate ResponseDelegate;
    ResponseDelegate.BindDynamic(this, &URequestFormWidget::OnResponse);
    const TArray<UButton*> Buttons { SendBtn.Get(), ConnectBtn.Get(), DisconnectBtn.Get() };
    UClientFormUtils::SetSelectedClient(Buttons, GameMode, SelectedType, SelectedOption, ResponseDelegate);
}

bool URequestFormWidget::SetClientAddress(UObject* Client)
{
    FString Host;
    int32 Port;
    bool IsSuccess = UClientFormUtils::TryParseAddressFields(HostField.Get(), PortField.Get(), Host, Port);
    if (IsSuccess) IClient::Execute_SetAddress(Client, Host, Port);
    return IsSuccess;
}
