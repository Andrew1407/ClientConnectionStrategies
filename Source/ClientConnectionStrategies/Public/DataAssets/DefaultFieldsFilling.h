// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DefaultFieldsFilling.generated.h"

/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UDefaultFieldsFilling : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultFieldsFilling")
    FString Host;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultFieldsFilling")
    int32 Port;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultFieldsFilling")
    TArray<int32> Denominations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultFieldsFilling")
    int32 Amount;

	UFUNCTION(BlueprintCallable, Category = "DefaultFieldsFilling|Formatters")
	FORCEINLINE FString PortStr() { return FString::FromInt(Port); }

	UFUNCTION(BlueprintCallable, Category = "DefaultFieldsFilling|Formatters")
	FORCEINLINE FString AmountStr() { return FString::FromInt(Amount); }

	UFUNCTION(BlueprintCallable, Category = "DefaultFieldsFilling|Formatters")
	FString DenominationsStr();
};
