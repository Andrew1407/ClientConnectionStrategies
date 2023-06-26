// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DefaultFieldsFilling.h"

FString UDefaultFieldsFilling::DenominationsStr() const
{
    const auto& NumStringifier = [] (const int32& Number) { return FString::Printf(TEXT("%d"), Number); };
    return FString::JoinBy(Denominations, TEXT(", "), NumStringifier);
}
