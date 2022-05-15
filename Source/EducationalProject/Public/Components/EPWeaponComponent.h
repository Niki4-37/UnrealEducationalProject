// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/EPBaseWeapon.h"

#include "EPWeaponComponent.generated.h"

class AEPBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDUCATIONALPROJECT_API UEPWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEPWeaponComponent();
    
    void Fire();
    /* use this info in widget*/
    bool GetAmmoData(FAmmoData& Data) const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<AEPBaseWeapon> Weapon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName WeaponAttachPointName = "WeaponSocket";
    
	virtual void BeginPlay() override;

private:
    UPROPERTY()
    AEPBaseWeapon* CurrentWeapon = nullptr;
    
    void SpawnWeapon();
};
