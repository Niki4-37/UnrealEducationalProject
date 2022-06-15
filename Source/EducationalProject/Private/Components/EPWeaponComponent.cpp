// For educational purposes only.

#include "Components/EPWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Player/EPBaseCharacter.h"
#include "AnimationHelper.h"

#include "Weapon/EPBaseWeapon.h"
#include "Weapon/EPMultiDamageWeapon.h"
#include "Weapon/EPPistol.h"
#include "Animation/EPIntoTheHolsterAnimNotify.h"
#include "Animation/EPFromTheHolsterAnimNotify.h"
#include "Animation/EPEquipFinishedAnimNotify.h"
#include "Animation/EPReloadFinishedAnimNotify.h"
#include "Animation/EPShellCasingEjectAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponent_LOG, All, All);

UEPWeaponComponent::UEPWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEPWeaponComponent::Fire()
{
    const auto Player = Cast<AEPBaseCharacter>(GetOwner());
    if (!CurrentWeapon || !Player) return;
    if (Player->IsAiming() && CanDoAction() && CurrentSingleFireAnimation)
    {
        CurrentWeapon->Fire();
        PlayAnimation(CurrentSingleFireAnimation);
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
    if (!CanEquip() || OwningWeapons.Num() < 2) return;
    /* find equip animation for current weapon */
    if (CurrentWeapon)
    {
        const auto CurrentWeaponAnimData = WeaponAnimData.FindByPredicate(                                //
            [&](const FWaeponAnimData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });  //
        CurrentEquipAnimation = CurrentWeaponAnimData ? CurrentWeaponAnimData->EquipAnimation : nullptr;
        /* CurrentEquipAnimation pointer checks inside PlayAnimMontage().
         * There is IntoTheHolsterAnimNotify in EquipAnimation */
        PlayAnimation(CurrentEquipAnimation);
        bEquipAnimInProgress = true;
    }
}

void UEPWeaponComponent::Reload()
{
    ChangeClip();
}

void UEPWeaponComponent::CanTakeObject(bool Enabled) 
{
    bCanTakeObject = Enabled;
}

void UEPWeaponComponent::TakeObject()
{
    if (!bCanTakeObject) return;
    /* Find and detach weapon with same WeaponType */
    int8 WeaponIndex = 0;
    for (auto Weapon : OwningWeapons)
    {
        if (DetachWeaponFromActor(Weapon, WeaponTypeAtPickup, WeaponIndex))
        {
            break;
        }
        ++WeaponIndex;
    }
    SpawnWeapon(WeaponClassAtPickup);
    PickupWasTaken.Broadcast();
    bCanTakeObject = false;
}

bool UEPWeaponComponent::TryToAddWeapon(TSubclassOf<AEPBaseWeapon> WaponClass, EWeaponType WeaponType)
{
    WeaponClassAtPickup = WaponClass;
    WeaponTypeAtPickup = WeaponType;

    return false;
}

void UEPWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAnimationNotify();
    CurrentWeaponIndex = 0;
    SpawnWeaponAtBeginPlay();
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

void UEPWeaponComponent::InitAnimationNotify()
{
    for (auto& OneWeaponAnim : WeaponAnimData)
    {
        auto IntoTheHolsterAnimNotify = AnimationHelper::FindNotifyByClass<UEPIntoTheHolsterAnimNotify>(OneWeaponAnim.EquipAnimation);
        if (!IntoTheHolsterAnimNotify)
        {
            UE_LOG(WeaponComponent_LOG, Error, TEXT("IntoTheHolsterAnimNotify not setted to current animation"));
            checkNoEntry();
        }
        if (!IntoTheHolsterAnimNotify->OnNotified.IsBoundToObject(this))
        {
            IntoTheHolsterAnimNotify->OnNotified.AddUObject(this, &UEPWeaponComponent::IntoTheHolster);
        }

        auto FromTheHolsterAnimNotify = AnimationHelper::FindNotifyByClass<UEPFromTheHolsterAnimNotify>(OneWeaponAnim.EquipAnimation);
        if (!FromTheHolsterAnimNotify)
        {
            UE_LOG(WeaponComponent_LOG, Error, TEXT("FromTheHolsterAnimNotify not setted to current animation"));
            checkNoEntry();
        }
        if (!FromTheHolsterAnimNotify->OnNotified.IsBoundToObject(this))
        {
            FromTheHolsterAnimNotify->OnNotified.AddUObject(this, &UEPWeaponComponent::FromTheHolster);
        }

        auto EquipFinishedAnimNotify = AnimationHelper::FindNotifyByClass<UEPEquipFinishedAnimNotify>(OneWeaponAnim.EquipAnimation);
        if (!EquipFinishedAnimNotify)
        {
            UE_LOG(WeaponComponent_LOG, Error, TEXT("EquipFinishedAnimNotify not setted to current animation"));
            checkNoEntry();
        }
        if (!EquipFinishedAnimNotify->OnNotified.IsBoundToObject(this))
        {
            EquipFinishedAnimNotify->OnNotified.AddUObject(this, &UEPWeaponComponent::OnEquipFinished);
        }
        
        auto ReloadFinishedAnimNotify = AnimationHelper::FindNotifyByClass<UEPReloadFinishedAnimNotify>(OneWeaponAnim.ReloadAnimation);
        if (!ReloadFinishedAnimNotify)
        {
            UE_LOG(WeaponComponent_LOG, Error, TEXT("ReloadFinishedAnimNotify not setted to current animation"));
            checkNoEntry();
        }
        if (!ReloadFinishedAnimNotify->OnNotified.IsBoundToObject(this))
        {
            ReloadFinishedAnimNotify->OnNotified.AddUObject(this, &UEPWeaponComponent::OnReloadFinished);
        }

        if (OneWeaponAnim.WeaponClass->IsChildOf<AEPMultiDamageWeapon>())
        {
            auto ShellCasingEjectAnimNotify = AnimationHelper::FindNotifyByClass<UEPShellCasingEjectAnimNotify>(OneWeaponAnim.ReloadAnimation);
            if (!ShellCasingEjectAnimNotify)
            {
                UE_LOG(WeaponComponent_LOG, Error, TEXT("ShellCasingEjectAnimNotify not setted to current animation"));
                checkNoEntry();
            }
            ShellCasingEjectAnimNotify->OnNotified.AddUObject(this, &UEPWeaponComponent::OnShellCasingEject);
        }
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
    UE_LOG(WeaponComponent_LOG, Display, TEXT("CurrentWeaponIndex: %i"), CurrentWeaponIndex);
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

void UEPWeaponComponent::OnShellCasingEject(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (MeshComponent != Character->GetMesh()) return;
    UE_LOG(WeaponComponent_LOG, Display, TEXT("OnShellCasingEject Event"));
    CurrentWeapon->SpawnEjectFX();
}

void UEPWeaponComponent::SpawnWeaponAtBeginPlay()
{
    for (const auto& OneWeaponAnim : WeaponAnimData)
    {
        if (OneWeaponAnim.WeaponClass->IsChildOf<AEPPistol>())
        {
            SpawnWeapon(OneWeaponAnim.WeaponClass);
        }
    }
}

void UEPWeaponComponent::SpawnWeapon(TSubclassOf<AEPBaseWeapon> WaponClass)
{
    ACharacter* Player = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Player || !Player->GetMesh()) return;

    auto Weapon = GetWorld()->SpawnActor<AEPBaseWeapon>(WaponClass);
    if (!Weapon) return;

    Weapon->OnClipEmpty.AddUObject(this, &UEPWeaponComponent::OnClipEmpty);
    Weapon->SetOwner(Player);
    OwningWeapons.Add(Weapon);
    ////////////////////////////////////////////////////////////////////////////
    WeaponArmorySocketName = Weapon->GetWeaponType() == EWeaponType::OneHand  //
                                 ? "PistolSocket"                             //
                                 : "ArmorySocket";                            //

    AttachWeaponToSocket(Weapon, Player->GetMesh(), WeaponArmorySocketName);
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

    const auto CurrentWeaponAnimData = WeaponAnimData.FindByPredicate([&](const FWaeponAnimData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });
    CurrentReloadAnimation = CurrentWeaponAnimData ? CurrentWeaponAnimData->ReloadAnimation : nullptr;
    CurrentSingleFireAnimation = CurrentWeaponAnimData ? CurrentWeaponAnimData->SingleFireAnimation : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Player->GetMesh(), WeaponArmedSocketName);
}

bool UEPWeaponComponent::DetachWeaponFromActor(AEPBaseWeapon* Weapon, EWeaponType Type, int8 WeaponIndex)
{
    if (!Weapon ||                          //
        Weapon->GetWeaponType() != Type ||  //
        !Weapon->GetSkeletalMesh() ||       //
        WeaponIndex > OwningWeapons.Num())  //
        return false;

    Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    Weapon->GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    Weapon->GetSkeletalMesh()->SetSimulatePhysics(true);

    if (Weapon == CurrentWeapon)
    {
        NextWeapon();
    }

    OwningWeapons.RemoveAt(WeaponIndex);
    return true;
}

bool UEPWeaponComponent::CanEquip()
{
    return CanDoAction();
}

bool UEPWeaponComponent::CanReload()
{
    return CurrentWeapon && CanDoAction() && CurrentWeapon->CanReload();
}

bool UEPWeaponComponent::CanDoAction()
{
    return !bEquipAnimInProgress && !bReloadAnimInProgress;
}

void UEPWeaponComponent::OnClipEmpty()
{
    ChangeClip();
}

void UEPWeaponComponent::ChangeClip()
{
    if (!CanReload()) return;
    CurrentWeapon->ChangeClip();
    bReloadAnimInProgress = true;
    PlayAnimation(CurrentReloadAnimation);
}
