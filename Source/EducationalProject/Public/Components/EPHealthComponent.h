// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, FVector, FName);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDUCATIONALPROJECT_API UEPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEPHealthComponent();
    
    float GetHealth() const { return Health; };

    bool IsDead() const { return Health <= 0.f; };

    FOnDeathSignature OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return Health / MaxHealth; };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float MaxHealth = 100.f;

	virtual void BeginPlay() override;

private:
    float Health = 0.f;
	
    void SetHaelth(float NewHealth);

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION()
    void OnTakePointDamage (AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

    UFUNCTION()
    void OnDestroyed(AActor* DestroyedActor);

    /* reaction on bullet impact */
    float CurrentHitBlend = 0.f;
    FTimerHandle BlendingTime;
    FName CurrentBoneName = NAME_None;

    void StartPhisicsReaction(AActor* DamagedActor, const FVector& ShotFromDirection, const FName& BoneName);
    void UpdateSimulation();
    void StopCurrentBoneSimulation();

};
