// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EPBaseWeapon.generated.h"

class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1", ClampMax = "45"))
        int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1", ClampMax = "10"))
        int32 Clips;
};

UCLASS()
class EDUCATIONALPROJECT_API AEPBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPBaseWeapon();

    virtual void Fire();
    /* use in WeaponComponent gives info about ammo to widget */
    FAmmoData GetCurrentAmmo() const { return CurrentAmmo; };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletFlyMaxDistace = 1500.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{ 10, 5 };

    /* Settings for current weapon type*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletForce = 1000.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float WeaponDamage = 10.f;

	virtual void BeginPlay() override;

    APlayerController* GetPlayerController() const;
    bool GetPlayerVeiwpoint(FVector& VeiwLocation, FRotator& VeiwRotation) const;
    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    void MakeShot();

private:
    FAmmoData CurrentAmmo;

    void DecreaseAmmo();
    bool IsClipEmpty() const;
    bool IsAmmoEmpty() const;
    void ChangeClip();
};
