#pragma once

UENUM(BlueprintType)
enum class EClientLabels : uint8
{
    NONE UMETA(DisplayName="NONE"),
    HTTP UMETA(DisplayName="HTTP"),
    WS UMETA(DisplayName="WS"),
    UDP UMETA(DisplayName="UDP"),
    TCP UMETA(DisplayName="TCP"),
};
