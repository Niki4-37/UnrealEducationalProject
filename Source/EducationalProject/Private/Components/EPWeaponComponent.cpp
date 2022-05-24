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
    if (Player->IsAiming())
    {
        CurrentWeapon->Fire();
    }
}

/* use decorate boolean function, which takes reference as a param to rewrites info about Current Weapon ammo */
bool UEPWeaponComponent::GetAmmoData(FAmmoData& Data) const
{
    if (CurrentWeapon)
    {
        Data = CurrentWeapon->GetCurrentAmmo();
        return true;
    }
    return false;
}

void UEPWeaponComponent::NextWeapon() 
{
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % OwningWeapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void UEPWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWeaponIndex = 0;
    SpawnWeapon();
    EquipWeapon(CurrentWeaponIndex);
}

void UEPWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
    CurrentWeapon = nullptr;
    for (auto Weapon : OwningWeapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy(); // there is no reason to stay lying weapon
    }

    OwningWeapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void UEPWeaponComponent::SpawnWeapon()
{
    ACharacter* Player = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Player || !Player->GetMesh()) return;

    for (auto OneWeaponAnim : WeaponAnimData)
    {
        auto Weapon = GetWorld()->SpawnActor<AEPBaseWeapon>(OneWeaponAnim.WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(Player);
        OwningWeapons.Add(Weapon);
    
        AttachWeaponToSocket(Weapon, Player->GetMesh(), WeaponArmorySocketName);

    }
}

void UEPWeaponComponent::AttachWeaponToSocket(AEPBaseWeapon* Weapon,            //
                                              USceneComponent* SceneComponent,  //
                                              const FName& SocketName)          //
{
    if (!Weapon || !SceneComponent) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UEPWeaponComponent::EquipWeapon(int32 WeaponIndex) 
{
    if (WeaponIndex < 0 || WeaponIndex > OwningWeapons.Num())
    {
        return;
    }
    
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player || !Player->GetMesh()) return;
    
    if (CurrentWeapon)
    {
        AttachWeaponToSocket(CurrentWeapon, Player->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = OwningWeapons[WeaponIndex];
    
    /*
    const auto CurrentWeaponAnimData = WeaponAnimData.FindByPredicate([&](const FWaeponAnimData& Data) { 
        return Data.WeaponClass == CurrentWeapon->GetClass();});

    CurrentAnimationSequences = CurrentWeaponAnimData->AnimSequences; ///////////
    */    

    AttachWeaponToSocket(CurrentWeapon, Player->GetMesh(), WeaponArmedSocketName);
 }
