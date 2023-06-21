// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ClientFormSwitch.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "GameFramework/HUD.h"
#include "UI/ClientFormViewer.h"


void UClientFormSwitch::NativeConstruct()
{
    Super::NativeConstruct();

    if (IsValid(ShowFormBtn)) ShowFormBtn->OnClicked.AddDynamic(this, &UClientFormSwitch::ShowClientForm);
}

void UClientFormSwitch::ShowClientForm()
{
    AHUD* CurrentHUD = GetOwningPlayer()->GetHUD();
    bool IsViewer = UKismetSystemLibrary::DoesImplementInterface(CurrentHUD, UClientFormViewer::StaticClass());
    if (!(IsValid(CurrentHUD) || IsViewer)) return;
    IClientFormViewer::Execute_ShowForm(CurrentHUD);
}
