// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPCoreData.h"
#include "EPBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPBaseWeapon();

    virtual void Fire();
    /* Gives info about ammo in widget use in WeaponComponent */
    FAmmoData GetCurrentAmmo() const { return CurrentAmmo; };
    
    UFUNCTION(BlueprintCallable, Category = "Animation")
    EWeaponType GetWeaponType() const { return WeaponType; };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletFlyMaxDistace = 1500.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{ 10, 5 };

    /* Use it to add impulse to body */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletForce = 1000.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float WeaponDamage = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType = EWeaponType::None;

	virtual void BeginPlay() override;

    APlayerController* GetPlayerController() const;
    bool GetPlayerVeiwpoint(FVector& VeiwLocation, FRotator& VeiwRotation) const;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    FVector GetMuzzleWorldLocation() const;

    virtual void MakeShot();
    
    void MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd);

    bool IsAmmoEmpty() const;
    void DecreaseAmmo();

private:
    FAmmoData CurrentAmmo;

    bool IsClipEmpty() const;
    void ChangeClip();
};
