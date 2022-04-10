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
