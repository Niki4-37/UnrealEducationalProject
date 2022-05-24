#pragma once

#include "EPCoreData.generated.h"

/*============================= Weapon =============================*/
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    None = 0,
    OneHand,
    TwoHand
};

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1", ClampMax = "45"))
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1", ClampMax = "10"))
    int32 Clips;
};