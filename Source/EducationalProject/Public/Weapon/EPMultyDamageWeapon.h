// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EPBaseWeapon.h"
#include "EPMultyDamageWeapon.generated.h"

class AEPProjectile;

UCLASS()
class EDUCATIONALPROJECT_API AEPMultyDamageWeapon : public AEPBaseWeapon
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AEPProjectile> ProjectileClass;

    virtual void MakeShot() override;
};
