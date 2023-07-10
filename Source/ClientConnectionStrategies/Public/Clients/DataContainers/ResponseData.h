#pragma once

#include "ResponseData.generated.h"

USTRUCT(BlueprintType)
struct FResponseData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, meta = (ClampMin = 0, UIMin = 0))
    int32 Result;
};
