// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Clients/ClientLabels.h"
#include "ClientCollection.generated.h"

/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UClientCollection : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientCollection")
	FColor DefaultColor = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientCollection")
	FColor ErrorColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientCollection")
	TMap<EClientLabels, FColor> ClientColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientCollection")
	TMap<FName, EClientLabels> ClientNames;
};
