// For educational purposes only.


#include "Pickups/EPWeaponPickup.h"
#include "Components/EPHealthComponent.h"
#include "Components/EPWeaponComponent.h"

bool AEPWeaponPickup::GivePickup(APawn* Pawn)
{
    UE_LOG(LogTemp, Display, TEXT("Enter function GivePickup"));
    const auto HealthComponent = GetRequiredComponent<UEPHealthComponent>(Pawn);
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = GetRequiredComponent<UEPWeaponComponent>(Pawn);
    if (!WeaponComponent) return false;
    
    WeaponComponent
        ->CanTakeObject(true);
    /* bind to delegate */
    WeaponComponent->PickupWasTaken
        .AddUObject(this, &AEPWeaponPickup::DestroyPickup);

    return WeaponComponent->TryToAddWeapon(WaponClass, WeaponType);
}

void AEPWeaponPickup::BeginPlay() 
{
    Super::BeginPlay();
}

void AEPWeaponPickup::DestroyPickup()
{
    Destroy();
}
