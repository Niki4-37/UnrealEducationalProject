// For educational purposes only.

#include "Components/EPWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Player/EPBaseCharacter.h"

#include "Weapon/EPBaseWeapon.h"
#include "Animation/EPIntoTheHolsterAnimNotify.h"
#include "Animation/EPFromTheHolsterAnimNotify.h"
#include "Animation/EPEquipFinishedAnimNotify.h"
#include "Animation/EPReloadFinishedAnimNotify.h"

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
    if (!CanEquip()) return;
    /* find equip animation for current weapon */
    if (CurrentWeapon)
    {
        const auto CurrentWeaponAnimData = WeaponAnimData.FindByPredicate([&](const FWaeponAnimData& Data)
            { return Data.WeaponClass == CurrentWeapon->GetClass(); });
        CurrentEquipAnimation = CurrentWeaponAnimData ? CurrentWeaponAnimData->EquipAnimation : nullptr;

        /* CurrentEquipAnimation pointer checks inside PlayAnimMontage(). 
        * There is IntoTheHolsterAnimNotify in EquipAnimation, */
        PlayAnimation(CurrentEquipAnimation);
        bEquipAnimInProgress = true;
    }
}

void UEPWeaponComponent::Reload() 
{
    UE_LOG(LogTemp, Display, TEXT("Reload button pressed"));
    if (!CanReload()) return;
    UE_LOG(LogTemp, Display, TEXT("Reload checked, allowed"));
    bReloadAnimInProgress = true;
    PlayAnimation(CurrentReloadAnimation);
}

void UEPWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAnimation();
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
        Weapon->Destroy();  // there is no reason to stay lying weapon
    }

    OwningWeapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void UEPWeaponComponent::InitAnimation()
{
    for (auto& OneWeaponAnim : WeaponAnimData)
    {
        auto IntoTheHolsterAnimNotify = FindNotifyByClass<UEPIntoTheHolsterAnimNotify>(OneWeaponAnim.EquipAnimation);
        if (!IntoTheHolsterAnimNotify) continue;
        IntoTheHolsterAnimNotify->OnNotified.AddUObject(this, &UEPWeaponComponent::IntoTheHolster);
    }
    
    for (auto& OneWeaponAnim : WeaponAnimData)
    {
        auto FromTheHolsterAnimNotify = FindNotifyByClass<UEPFromTheHolsterAnimNotify>(OneWeaponAnim.EquipAnimation);
        if (!FromTheHolsterAnimNotify) continue;
        FromTheHolsterAnimNotify->OnNotified.AddUObject(this, &UEPWeaponComponent::FromTheHolster);
    }

    for (auto& OneWeaponAnim : WeaponAnimData)
    {
        auto EquipFinishedAnimNotify = FindNotifyByClass<UEPEquipFinishedAnimNotify>(OneWeaponAnim.EquipAnimation);
        if (!EquipFinishedAnimNotify) continue;
        EquipFinishedAnimNotify->OnNotified.AddUObject(this, &UEPWeaponComponent::OnEquipFinished);
    }

    for (auto& OneWeaponAnim : WeaponAnimData)
    {
        auto ReloadFinishedAnimNotify = FindNotifyByClass<UEPReloadFinishedAnimNotify>(OneWeaponAnim.ReloadAnimation);
        if (!ReloadFinishedAnimNotify) continue;
        ReloadFinishedAnimNotify->OnNotified.AddUObject(this, &UEPWeaponComponent::OnReloadFinished);
    }
}

void UEPWeaponComponent::PlayAnimation(UAnimMontage* Animation)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

/* place weapon to ArmorySocket when notify riched */
void UEPWeaponComponent::IntoTheHolster(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;

    if (CurrentWeapon)
    {
        ///////////////////////////////////////////////////////////////////////////////////
        WeaponArmorySocketName = CurrentWeapon->GetWeaponType() == EWeaponType::OneHand  //
                                     ? "PistolSocket"                                    //
                                     : "ArmorySocket";                                   //

        AttachWeaponToSocket(CurrentWeapon, MeshComponent, WeaponArmorySocketName);
    }
}

/* binded to FromTheHolsterAnimNotify, makes weapon change */
void UEPWeaponComponent::FromTheHolster(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % OwningWeapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

/* binded to EquipFinishedAnimNotify, allows weapon change */
void UEPWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;

    bEquipAnimInProgress = false;
}

void UEPWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent) 
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;

    bReloadAnimInProgress = false;
}

void UEPWeaponComponent::SpawnWeapon()
{
    ACharacter* Player = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Player || !Player->GetMesh()) return;

    for (const auto& OneWeaponAnim : WeaponAnimData)
    {
        auto Weapon = GetWorld()->SpawnActor<AEPBaseWeapon>(OneWeaponAnim.WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(Player);
        OwningWeapons.Add(Weapon);
        ////////////////////////////////////////////////////////////////////////////
        WeaponArmorySocketName = Weapon->GetWeaponType() == EWeaponType::OneHand  //
                                     ? "PistolSocket"                             //
                                     : "ArmorySocket";                            //

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
    if (WeaponIndex < 0 || WeaponIndex > OwningWeapons.Num()) return;

    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player || !Player->GetMesh()) return;

    CurrentWeapon = OwningWeapons[WeaponIndex];

    UE_LOG(LogTemp, Display, TEXT("CurrentWeapon %s"), *CurrentWeapon->GetName());
    
    const auto CurrentWeaponAnimData = WeaponAnimData.FindByPredicate([&](const FWaeponAnimData& Data)
        { return Data.WeaponClass == CurrentWeapon->GetClass(); });
    CurrentReloadAnimation = CurrentWeaponAnimData ? CurrentWeaponAnimData->ReloadAnimation : nullptr;
    
    FString TestText = CurrentReloadAnimation ? "true" : "false";
    UE_LOG(LogTemp, Display, TEXT("CurrentReloadAnimation %s"), *TestText);

    AttachWeaponToSocket(CurrentWeapon, Player->GetMesh(), WeaponArmedSocketName);
}

bool UEPWeaponComponent::CanEquip()
{
    return !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool UEPWeaponComponent::CanReload()
{
    return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}