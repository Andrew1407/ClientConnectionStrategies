#pragma once

#include "Containers/UnrealString.h"
#include "Clients/Interfaces/Connection.h"
#include "ClientOptions.generated.h"

USTRUCT(BlueprintType)
struct FClientOptions
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString Host;

    UPROPERTY(BlueprintReadWrite, meta = (ClampMin = 0, UIMin = 0))
    int32 Port;

    UPROPERTY(BlueprintReadWrite)
    FResponseDeledate ResponseDeledate;

    UPROPERTY(BlueprintReadWrite)
    float ResponseTimeout;
};
