// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPCoreData.h"
#include "EPBaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

class USkeletalMeshComponent;
class UAnimMontage;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;

    virtual void Fire();
    /* Gives info about ammo in widget use in WeaponComponent */
    FAmmoData GetCurrentAmmo() const { return CurrentAmmo; };
    /* Used in AnimationBlueprint */
    UFUNCTION(BlueprintCallable, Category = "Animation")
    EWeaponType GetWeaponType() const { return WeaponType; };

    bool CanReload() const;
    void ChangeClip();

    USkeletalMeshComponent* GetSkeletalMesh() const { return WeaponMesh; };
    
    UNiagaraComponent* SpawnEjectFX();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX")
    FName AmmoEjectSocketName = "AmmoEject";

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* ShootingAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* EjectFX;

	virtual void BeginPlay() override;

    APlayerController* GetPlayerController() const;
    bool GetPlayerVeiwpoint(FVector& VeiwLocation, FRotator& VeiwRotation) const;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    FVector GetMuzzleWorldLocation() const;

    virtual void MakeShot();
    
    void MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd);

    bool IsAmmoEmpty() const;
    void DecreaseAmmo();

    virtual void InitAnimation();

private:
    FAmmoData CurrentAmmo;

    bool IsClipEmpty() const;

    void PlayAnimation(UAnimationAsset* Animation);
};
