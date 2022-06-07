// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EPBaseWeapon.h"
#include "EPMultiDamageWeapon.generated.h"

class AEPProjectile;

UCLASS()
class EDUCATIONALPROJECT_API AEPMultiDamageWeapon : public AEPBaseWeapon
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AEPProjectile> ProjectileClass;

    virtual void MakeShot() override;
};
