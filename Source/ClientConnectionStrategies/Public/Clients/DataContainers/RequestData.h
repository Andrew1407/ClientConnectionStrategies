#pragma once

#include "RequestData.generated.h"

USTRUCT(BlueprintType)
struct FRequestData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite)
    TArray<int32> Denominations;

    UPROPERTY(BlueprintReadWrite, meta = (ClampMin = 0, UIMin = 0))
    int32 Amount;
};
