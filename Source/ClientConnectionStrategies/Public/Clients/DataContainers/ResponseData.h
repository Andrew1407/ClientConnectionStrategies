#pragma once

#include "ResponseData.generated.h"

USTRUCT(BlueprintType)
struct FResponseData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 Result;
};
