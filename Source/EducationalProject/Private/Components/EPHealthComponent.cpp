// For educational purposes only.

DEFINE_LOG_CATEGORY_STATIC(HealthComponent, All, All)

#include "Components/EPHealthComponent.h"
#include "GameFramework/Actor.h"

UEPHealthComponent::UEPHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UEPHealthComponent::BeginPlay()
{
	Super::BeginPlay();

    Health = MaxHealth;

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UEPHealthComponent::OnTakeAnyDamage);
    }
    
}

void UEPHealthComponent::SetHaelth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
}

void UEPHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    Health =- Damage;
    UE_LOG(HealthComponent, Display, TEXT("Health %f"), Health);
}


