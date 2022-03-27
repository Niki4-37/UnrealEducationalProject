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

	virtual void BeginPlay() override;

public:	
    virtual void Fire();

};
