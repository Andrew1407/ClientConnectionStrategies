// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClientLabels.h"
#include "ClientFactory.generated.h"

/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UClientFactory : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    class UHttpClient* CreateHttpClient(UObject* Owner, UPARAM(ref) EClientLabels& Label);

    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    class UWsClient* CreateWsClient(UObject* Owner, UPARAM(ref) EClientLabels& Label);

    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    class UUdpClient* CreateUdpClient(UObject* Owner, UPARAM(ref) EClientLabels& Label);

    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    class UTcpClient* CreateTcpClient(UObject* Owner, UPARAM(ref) EClientLabels& Label);

    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    UObject* CreateByTarget(EClientLabels Target, UObject* Owner, UPARAM(ref) EClientLabels& Label);
};