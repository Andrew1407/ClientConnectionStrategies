// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClientHUD.h"
#include "UI/RequestFormWidget.h"
#include "UI/ClientFormSwitch.h"
#include "GameFramework/PlayerController.h"

void AClientHUD::BeginPlay()
{
    Super::BeginPlay();

    CreateClientWidget();
    CreateSwitchWidget();
    if (IsValid(ClientFormSwitch)) ClientFormSwitch->SetVisibility(ESlateVisibility::Collapsed);

   ;
    APlayerController* PlayerController =  GetWorld()->GetFirstPlayerController();
    if (IsValid(PlayerController))
    {
        PlayerController->SetInputMode(FInputModeUIOnly());
        PlayerController->SetShowMouseCursor(true);
    }
}

void AClientHUD::CreateClientWidget()
{
    if (!IsValid(ClientWidgetClass) || IsValid(ClientRequestWidget)) return;
    ClientRequestWidget = CreateWidget<URequestFormWidget>(GetWorld(), ClientWidgetClass);
    ClientRequestWidget->AddToViewport();
}

void AClientHUD::CreateSwitchWidget()
{
    if (!IsValid(ClientFormSwitchClass) || IsValid(ClientFormSwitch)) return;
    ClientFormSwitch = CreateWidget<UClientFormSwitch>(GetWorld(), ClientFormSwitchClass);
    ClientFormSwitch->AddToViewport();
}

void AClientHUD::ShowForm_Implementation()
{
    if (!(IsValid(ClientRequestWidget) && IsValid(ClientRequestWidget))) return;
    ClientFormSwitch->SetVisibility(ESlateVisibility::Collapsed);
    ClientRequestWidget->SetVisibility(ESlateVisibility::Visible);
}

void AClientHUD::HideForm_Implementation()
{
    if (!(IsValid(ClientRequestWidget) && IsValid(ClientRequestWidget))) return;
    ClientRequestWidget->SetVisibility(ESlateVisibility::Collapsed);
    ClientFormSwitch->SetVisibility(ESlateVisibility::Visible);
}

