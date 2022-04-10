// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SkeletalMeshComponent.h"

#include "EPBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPBaseWeapon();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletFlyMaxDistace = 1500.f;

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

public:	
    virtual void Fire();

};
