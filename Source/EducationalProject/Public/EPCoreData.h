#pragma once

#include "EPCoreData.generated.h"

class AEPBaseWeapon;

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

/*========================== WeaponComponent ==========================*/
USTRUCT(BlueprintType)
struct FWaeponAnimData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    TSubclassOf<AEPBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* EquipAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* ReloadAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* SingleFireAnimation;
};