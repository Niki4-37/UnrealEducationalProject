// For educational purposes only.


#include "Components/EPWeaponComponent.h"
#include "Player/EPBaseCharacter.h"

UEPWeaponComponent::UEPWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UEPWeaponComponent::Fire()
{
    const auto Player = Cast<AEPBaseCharacter>(GetOwner());
    if (!CurrentWeapon || !Player) return;
    if (Player->IsAiming()) CurrentWeapon->Fire();
}

/* use decorate bool function, which takes reference as a param to rewrites info about Current Weapon ammo */
bool UEPWeaponComponent::GetAmmoData(FAmmoData& Data) const
{
    if (CurrentWeapon)
    {
        Data = CurrentWeapon->GetCurrentAmmo();
        return true;
    }
    return false;
}

void UEPWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    SpawnWeapon();
}

void UEPWeaponComponent::SpawnWeapon()
{
    ACharacter* Player = Cast<ACharacter>(GetOwner());
    
    if (!GetWorld() || !Player) return;

    CurrentWeapon = GetWorld()->SpawnActor<AEPBaseWeapon>(Weapon);
    if (!CurrentWeapon) return;
    
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Player->GetMesh(), AttachmentRules, WeaponAttachPointName);
    CurrentWeapon->SetOwner(Player);
}
