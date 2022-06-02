// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EPProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UDamageType;

UCLASS()
class EDUCATIONALPROJECT_API AEPProjectile : public AActor
{
    GENERATED_BODY()

public:
    AEPProjectile();

    void SetProjectileDirection(const FVector& Derection) { ProjectileDirection = Derection; };

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float LifeTime = 5.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.f;

    virtual void BeginPlay() override;

private:
    FVector ProjectileDirection;

    UFUNCTION()
    void OnComponentHit(UPrimitiveComponent* HitComponent,   //
                        AActor* OtherActor,                  //
                        UPrimitiveComponent* OtherComp,      //
                        FVector NormalImpulse,               //
                        const FHitResult& Hit);              //

    AController* GetController() const;

};
