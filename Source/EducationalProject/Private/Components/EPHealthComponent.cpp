// For educational purposes only.

DEFINE_LOG_CATEGORY_STATIC(HealthComponent, All, All)

#include "Components/EPHealthComponent.h"
#include "GameFramework/Actor.h"
/* Add damage types */
#include "DamageFactors/EPFireDamageType.h"
/* */
#include "EPGameModeBase.h"
#include "GameFramework/Character.h"

UEPHealthComponent::UEPHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEPHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    /* to prevent zero division in GetHealthPercent() */
    check(MaxHealth > 0);

    check(GetWorld());
    SetHaelth(MaxHealth);
    /* GetOwner() ActorComponent's class method gets pointer to Actor which belongs this component */
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        /* Get Actor's delegate and bind (AddDynamic for Multicast Delegate) local function OnTakeAnyDamage(...) */
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UEPHealthComponent::OnTakeAnyDamage);

        ComponentOwner->OnTakePointDamage.AddDynamic(this, &UEPHealthComponent::OnTakePointDamage);
        /* Bind to this delegate to use in spawning options */
        ComponentOwner->OnDestroyed.AddDynamic(this, &UEPHealthComponent::OnDestroyed);
    }
}

void UEPHealthComponent::SetHaelth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
}

/* Create local function with equal params which difined in Actor's class
FTakeAnyDamageSignature OnTakeAnyDamage delegate */
void UEPHealthComponent::OnTakeAnyDamage(AActor* DamagedActor,                 //
                                         float Damage,                         //
                                         const class UDamageType* DamageType,  //
                                         class AController* InstigatedBy,      //
                                         AActor* DamageCauser)                 //
{
    if (Damage < 0.f || IsDead()) return;

    ApplyDamage(Damage);
    
    /* Check damage type */
    if (DamageType)
    {
        if (DamageType->IsA<UEPFireDamageType>())
        {
            UE_LOG(HealthComponent, Display, TEXT("Gain Fire Damage!"));
        }
    }
}

void UEPHealthComponent::OnTakePointDamage(AActor* DamagedActor,                      //
                                           float Damage,                              //
                                           class AController* InstigatedBy,           //
                                           FVector HitLocation,                       //
                                           class UPrimitiveComponent* FHitComponent,  //
                                           FName BoneName,                            //
                                           FVector ShotFromDirection,                 //
                                           const class UDamageType* DamageType,       //
                                           AActor* DamageCauser)                      //
{
    ApplyDamage(Damage);

    if (IsDead())
    {
        const auto Character = Cast<ACharacter>(DamagedActor);
        if (!Character) return; 
        const auto Mass = Character->GetMesh()->GetBoneMass(BoneName);
        const FVector Force = FMath::IsNearlyZero(Mass) ? FVector::ZeroVector : ((HitLocation - ShotFromDirection).GetSafeNormal() * 5000.f / Mass);
        Character->GetMesh()->SetPhysicsLinearVelocity(Force, true, BoneName);
    }
    if (!IsDead() && BoneName != "pelvis")  // temporary plug
    {
        // StartPhisicsReaction(DamagedActor, ShotFromDirection, BoneName);
    }
}

void UEPHealthComponent::OnDestroyed(AActor* DestroyedActor)
{
    /* GameMode function StartSpawningProcess() starts TimerManager with respawning function */
    if (!GetWorld()) return;
    const auto GameMode = GetWorld()->GetAuthGameMode<AEPGameModeBase>();
    if (!GameMode) return;
    GameMode->StartSpawningProcess();
}

void UEPHealthComponent::ApplyDamage(float Damage) 
{
    SetHaelth(Health - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();
        UE_LOG(LogTemp, Display, TEXT("Character is Dead!"));
    }
}

void UEPHealthComponent::StartPhisicsReaction(AActor* DamagedActor, const FVector& ShotFromDirection, const FName& BoneName)
{
    /* Cast<>() checks DamagedActor nullptr */
    // const auto Character = Cast<ACharacter>(DamagedActor);
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || !Character->GetMesh()) return;

    if (!CurrentBoneName.IsNone())
    {
        StopCurrentBoneSimulation();
    }
    CurrentHitBlend = 0.3f;
    CurrentBoneName = BoneName;

    const float ImpulseForce = 1.f;  // make this option in weapon class

    Character->GetMesh()->SetAllBodiesBelowSimulatePhysics(CurrentBoneName, true);
    Character->GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(CurrentBoneName, CurrentHitBlend);
    Character->GetMesh()->AddImpulseToAllBodiesBelow(ShotFromDirection * ImpulseForce, CurrentBoneName, true);

    const float TimerRate = 0.01f;
    const bool bIsRepeat = true;
    GetWorld()->GetTimerManager().ClearTimer(BlendingTime);
    GetWorld()->GetTimerManager().SetTimer(BlendingTime, this, &UEPHealthComponent::UpdateSimulation, TimerRate, bIsRepeat);
}

void UEPHealthComponent::UpdateSimulation()
{
    CurrentHitBlend -= 0.1f;

    if (FMath::IsNearlyZero(CurrentHitBlend))
        if (CurrentHitBlend <= 0.f)
        {
            GetWorld()->GetTimerManager().ClearTimer(BlendingTime);
            StopCurrentBoneSimulation();
        }
        else
        {
            const auto Character = Cast<ACharacter>(GetOwner());
            if (!Character || !Character->GetMesh()) return;

            Character->GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(CurrentBoneName, CurrentHitBlend);
        }
}

void UEPHealthComponent::StopCurrentBoneSimulation()
{
    CurrentBoneName = NAME_None;
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || !Character->GetMesh()) return;

    Character->GetMesh()->SetAllBodiesBelowSimulatePhysics(CurrentBoneName, false);
    Character->GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(CurrentBoneName, 0.f);
}
