// For educational purposes only.

DEFINE_LOG_CATEGORY_STATIC(HealthComponent, All, All)

#include "Components/EPHealthComponent.h"
#include "GameFramework/Actor.h"
/* Add damage types */
#include "DamageFactors/EPFireDamageType.h"

UEPHealthComponent::UEPHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UEPHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    /* to prevent zero division in GetHealthPercent() */
    check(MaxHealth > 0);
    SetHaelth(MaxHealth);
    /* GetOwner() ActorComponent's class method gets pointer to Actor which belongs this component */
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        /* Get Actor's delegate and bind (AddDynamic for Multicast Delegate) local function OnTakeAnyDamage(...) */
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UEPHealthComponent::OnTakeAnyDamage);

        ComponentOwner->OnTakePointDamage.AddDynamic(this, &UEPHealthComponent::OnTakePointDamage);
    }
    
}

void UEPHealthComponent::SetHaelth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
}

/* Create local function with equal params which difined in Actor's FTakeAnyDamageSignature OnTakeAnyDamage delegate*/
void UEPHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage < 0.f || IsDead()) return;
    
    Health = FMath::Clamp( Health - Damage, 0.f, MaxHealth);
    
    /* Check damage type */
    if (DamageType)
    {
        if (DamageType->IsA<UEPFireDamageType>())
        {
            UE_LOG(HealthComponent, Display, TEXT("Gain Fire Damage!"));
        }
    }

    //if (IsDead())
    //{
    //    OnDeath.Broadcast(FVector::ZeroVector, FName("none"));
    //}
}

void UEPHealthComponent::OnTakePointDamage (
    AActor* DamagedActor,
    float Damage, 
    class AController* InstigatedBy,
    FVector HitLocation,
    class UPrimitiveComponent* FHitComponent,
    FName BoneName, 
    FVector ShotFromDirection,
    const class UDamageType* DamageType, 
    AActor* DamageCauser)
{
    if (IsDead())
    {
        OnDeath.Broadcast(ShotFromDirection, BoneName);
    }
}

