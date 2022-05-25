// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EPProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPProjectile();

    void SetProjectileDirection(const FVector& Derection){ProjectileDirection = Derection; };

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    UProjectileMovementComponent* MovementComponent;

	virtual void BeginPlay() override;

private:
    FVector ProjectileDirection;
};
