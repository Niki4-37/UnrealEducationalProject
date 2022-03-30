// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDUCATIONALPROJECT_API UEPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEPHealthComponent();
    
    float GetHealth() const { return Health; };

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return Health / MaxHealth; };

protected:
    UPROPERTY(EditdefaultsOnly, BlueprintReadWrite)
    float MaxHealth = 100.f;

	virtual void BeginPlay() override;

private:
    float Health = 0.f;
	
    void SetHaelth(float NewHealth);

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
