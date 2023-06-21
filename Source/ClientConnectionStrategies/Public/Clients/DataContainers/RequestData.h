#pragma once

#include "RequestData.generated.h"

USTRUCT(BlueprintType)
struct FRequestData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TArray<int32> Denominations;

    UPROPERTY(BlueprintReadWrite)
    int32 Amount;
};
