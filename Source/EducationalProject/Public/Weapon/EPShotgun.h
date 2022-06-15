// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EPBaseWeapon.h"
#include "EPShotgun.generated.h"

class AEPProjectile;

UCLASS()
class EDUCATIONALPROJECT_API AEPShotgun : public AEPBaseWeapon
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<AEPProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float ProjectileSpread = 5.f;

    virtual void MakeShot() override;
};
