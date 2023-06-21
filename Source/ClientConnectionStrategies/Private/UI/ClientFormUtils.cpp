// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClientFormUtils.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"

#include "Clients/DataContainers/RequestData.h"
#include "Clients/DataContainers/ResponseData.h"

#include "Clients/Interfaces/Connection.h"
#include "Clients/Interfaces/ClientContainer.h"

bool UClientFormUtils::ValidateFields(const TMap<UEditableTextBox*, FString>& FieldValidators)
{
    for (const auto& Pair : FieldValidators)
    {
        const FRegexPattern RegexPattern(Pair.Value);
        FRegexMatcher Matcher(RegexPattern, Pair.Key->GetText().ToString());
        if (!Matcher.FindNext()) return false;
    }
    return true;
}

void UClientFormUtils::SetSelectedClient(const TArray<UButton*>& Buttons, AGameModeBase* GameMode,
    EClientLabels SelectedType, const FString& SelectedOption, const FResponseDeledate& ResponseDeledate)
{
    if (!IClientContainer::Execute_SetClient(GameMode, SelectedType)) return;
    UObject* Client = IClientContainer::Execute_GetClient(GameMode);
    bool IsClient = UKismetSystemLibrary::DoesImplementInterface(Client, UClient::StaticClass());
    if (!IsClient) return;

    IClient::Execute_SetResponseDelegate(Client, ResponseDeledate);

    bool IsConnection = UKismetSystemLibrary::DoesImplementInterface(Client, UConnection::StaticClass());
    ESlateVisibility BtnVisibility = !IsConnection ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
    const TObjectPtr<UButton>& SendBtn = Buttons[0];
    const TObjectPtr<UButton>& ConnectBtn = Buttons[1];
    const TObjectPtr<UButton>& DisconnectBtn = Buttons[2];

    const TMap<UButton*, const FString> ButtonTips {
        { SendBtn, "Send data via {0} client" },
        { ConnectBtn, "Connect via {0} client" },
        { DisconnectBtn, "Disonnect via {0} client" },
    };
    for (const auto& Pair : ButtonTips)
    {
        const auto& Button = Pair.Key;
        bool IsConnectBtn = ConnectBtn == Button;
        bool IsSendBtn = SendBtn == Button;
        Button->SetIsEnabled(!IsConnection || IsConnectBtn);
        Button->SetVisibility(IsSendBtn ? ESlateVisibility::Visible : BtnVisibility);
        if (IsConnection || IsSendBtn)
            Button->SetToolTipText(FText::FromString(FString::Format(*Pair.Value, { *SelectedOption })));
    }
}

bool UClientFormUtils::TryParseAddressFields(UEditableTextBox* HostField, UEditableTextBox* PortField, FString& Host, int32& Port)
{
    bool IsFieldsValid = ValidateFields({
        { HostField, "^((localhost)|(\\d+\\.){3}\\d+)$" },
        { PortField, "^\\d+$" },
    });
    if (!IsFieldsValid) return false;
    Host = HostField->GetText().ToString();
    Port = FCString::Atoi(*PortField->GetText().ToString());
    return true;
}

bool UClientFormUtils::TryParseParamsFields(UEditableTextBox* FirstParamField, UEditableTextBox* SecondParamField, TArray<int32>& Denominations, int32& Amount)
{
    bool IsFieldsValid = ValidateFields({
        { FirstParamField, "^\\d+(,\\s?\\d+)*$" },
        { SecondParamField, "^\\d+$" },
    });
    if (!IsFieldsValid) return false;
    TArray<FString> Substrings;
    FirstParamField->GetText().ToString().ParseIntoArray(Substrings, TEXT(","), true);
    TSet<int32> DenominationsSet;
    for (const auto& Substring : Substrings)
        DenominationsSet.Add(FCString::Atoi(*Substring.TrimStartAndEnd()));
    Denominations = DenominationsSet.Array();
    Amount = FCString::Atoi(*SecondParamField->GetText().ToString());
    return true;
}

void UClientFormUtils::HandleConnectionAction(AGameModeBase* GameMode, const TObjectPtr<UButton>& Button, TFunctionRef<bool(UObject*)> ConnectionCheck)
{
    if (!IsValid(GameMode)) return;
    UObject* Client = IClientContainer::Execute_GetClient(GameMode);
    bool bHasConnection = UKismetSystemLibrary::DoesImplementInterface(Client, UConnection::StaticClass());
    Button->SetIsEnabled(false);
    bool IsActionValid = bHasConnection && ConnectionCheck(Client);
    if (!IsActionValid) Button->SetIsEnabled(true);
}
