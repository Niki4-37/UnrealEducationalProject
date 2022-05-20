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
    const auto HalfAngleSpreadRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShotDirection = FMath::VRandCone(VeiwRotation.Vector(), HalfAngleSpreadRad); 
    TraceEnd = VeiwLocation + ShotDirection * BulletFlyMaxDistace;

    return true;
}

void AEPBaseWeapon::MakeShot()
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Player || IsAmmoEmpty()) return;
    const auto Controller = GetPlayerController();
    if (!Controller) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FCollisionQueryParams CollisionParams;

    FHitResult HitResult;

    GetWorld()->LineTraceSingleByChannel(HitResult,                          //
                                         TraceStart,                         //
                                         TraceEnd,                           //
                                         ECollisionChannel::ECC_Visibility,  //
                                         CollisionParams);                   //

    if (HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(),                                       //
                      WeaponMesh->GetSocketLocation(MuzzleSocketName),  //
                      HitResult.ImpactPoint,                            //
                      FColor::Cyan,                                     //
                      false,                                            //
                      5.f);                                             //

        DrawDebugSphere(GetWorld(),             //
                        HitResult.ImpactPoint,  //
                        5.f,                    //
                        24,                     //
                        FColor::Blue,           //
                        false,                  //
                        5.f);                   //

        const auto DamageActor = HitResult.GetActor();
        if (!DamageActor) return;
        UE_LOG(LogTemp, Display, TEXT("ActorDamaged! %s"), *HitResult.BoneName.ToString());

        UGameplayStatics::ApplyPointDamage(DamageActor,   //
                                           WeaponDamage,  //
                                           Player->GetActorLocation(),  //
                                           HitResult,     //
                                           Controller,    //
                                           Player,        //
                                           nullptr);      // TSubclassOf<UDamageType> DamageTypeClass
    }
    else
    {
        DrawDebugLine(GetWorld(),                                       //
                      WeaponMesh->GetSocketLocation(MuzzleSocketName),  //
                      TraceEnd,                                         //
                      FColor::Purple,                                   //
                      false,                                            //
                      5.f);                                             //
    }

    DecreaseAmmo();
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

bool AEPBaseWeapon::IsAmmoEmpty() const
{
    return IsClipEmpty() && CurrentAmmo.Clips == 0;
}

void AEPBaseWeapon::ChangeClip()
{
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    --CurrentAmmo.Clips;
}
