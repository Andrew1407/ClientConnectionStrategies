#pragma once

#include "UObject/NameTypes.h"
#include "Containers/UnrealString.h"
#include "ConnectionType.generated.h"

USTRUCT(BlueprintType)
struct FConnectionType
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName Name;

    UPROPERTY(BlueprintReadWrite)
    FString Description;
};
