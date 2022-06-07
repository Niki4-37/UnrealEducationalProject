// For educational purposes only.

#include "Weapon/EPBaseWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeapon_LOG, All, All);

AEPBaseWeapon::AEPBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void AEPBaseWeapon::Fire()
{
    MakeShot();
}

bool AEPBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

/* Additional uses in WeaponComponent */
void AEPBaseWeapon::ChangeClip()
{
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    --CurrentAmmo.Clips;
}

UNiagaraComponent* AEPBaseWeapon::SpawnEjectFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(EjectFX,                        //
                                                        WeaponMesh,                     //
                                                        AmmoEjectSocketName,            //
                                                        FVector::ZeroVector,            //
                                                        FRotator::ZeroRotator,          //
                                                        EAttachLocation::SnapToTarget,  //
                                                        true);                          //
}

void AEPBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    check(GetWorld());

    CurrentAmmo = DefaultAmmo;

    InitAnimation();
}

APlayerController* AEPBaseWeapon::GetPlayerController() const
{
    /* To get PlayerController of this Weapon needs several steps:
        GetOwner() returns Actor to make Cast to Character to get it's controller,
        check pointer, return Controller */
    const auto Player = Cast<ACharacter>(GetOwner());
    return Player ? Player->GetController<APlayerController>() : nullptr;
}

bool AEPBaseWeapon::GetPlayerVeiwpoint(FVector& VeiwLocation, FRotator& VeiwRotation) const
{
    if (!GetPlayerController()) return false;
    GetPlayerController()->GetPlayerViewPoint(VeiwLocation, VeiwRotation);

    return true;
}

bool AEPBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector VeiwLocation;
    FRotator VeiwRotation;
    if (!GetPlayerVeiwpoint(VeiwLocation, VeiwRotation)) return false;

    TraceStart = VeiwLocation;
    const FVector ShotDirection = VeiwRotation.Vector();
    TraceEnd = VeiwLocation + ShotDirection * BulletFlyMaxDistace;

    return true;
}

FVector AEPBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

void AEPBaseWeapon::MakeShot() 
{
    PlayAnimation(ShootingAnimation);
}

void AEPBaseWeapon::MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult,                          //
                                         TraceStart,                         //
                                         TraceEnd,                           //
                                         ECollisionChannel::ECC_Visibility,  //
                                         CollisionParams);                   //
}

bool AEPBaseWeapon::IsAmmoEmpty() const
{
    return IsClipEmpty() && CurrentAmmo.Clips == 0;
}

void AEPBaseWeapon::DecreaseAmmo()
{
    --CurrentAmmo.Bullets;
    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        // ChangeClip();
        OnClipEmpty.Broadcast();
    }
}

void AEPBaseWeapon::InitAnimation() {}

bool AEPBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void AEPBaseWeapon::PlayAnimation(UAnimationAsset* Animation)
{
    WeaponMesh->PlayAnimation(Animation, false);
}

