// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/EPBasePickup.h"
#include "EPCoreData.h"

#include "EPWeaponPickup.generated.h"

class AEPBaseWeapon;

UCLASS()
class EDUCATIONALPROJECT_API AEPWeaponPickup : public AEPBasePickup
{
	GENERATED_BODY()

public:
    virtual bool GivePickup(APawn* Pawn) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
    TSubclassOf<AEPBaseWeapon> WaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    EWeaponType WeaponType = EWeaponType::None;
    
    virtual void BeginPlay() override;

    void DestroyPickup();
	
};
