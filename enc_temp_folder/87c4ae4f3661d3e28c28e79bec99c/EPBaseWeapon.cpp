// For educational purposes only.

#include "Weapon/EPBaseWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine.h"

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

void AEPBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);

    CurrentAmmo = DefaultAmmo;
}

APlayerController* AEPBaseWeapon::GetPlayerController() const
{
    /* To get PlayerController of this Weapon needs several steps:
        GetOwner() returns Actor to make Cast to Character to get it's controller,
        check pointer, return Controller */
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;
    return Player->GetController<APlayerController>();
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

void AEPBaseWeapon::MakeShot()
{
    
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
        ChangeClip();
    }
}

bool AEPBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void AEPBaseWeapon::ChangeClip()
{
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    --CurrentAmmo.Clips;
}
