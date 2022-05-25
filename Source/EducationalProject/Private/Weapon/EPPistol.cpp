// For educational purposes only.

#include "Weapon/EPPistol.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

bool AEPPistol::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
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

void AEPPistol::MakeShot()
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Player || IsAmmoEmpty()) return;
    const auto Controller = GetPlayerController();
    if (!Controller) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(),                //
                      GetMuzzleWorldLocation(),  //
                      HitResult.ImpactPoint,     //
                      FColor::Cyan,              //
                      false,                     //
                      5.f);                      //

        DrawDebugSphere(GetWorld(),             //
                        HitResult.ImpactPoint,  //
                        5.f,                    //
                        24,                     //
                        FColor::Blue,           //
                        false,                  //
                        5.f);                   //

        const auto DamageActor = HitResult.GetActor();
        if (!DamageActor) return;
        // UE_LOG(LogTemp, Display, TEXT("ActorDamaged! %s"), *HitResult.BoneName.ToString());

        UGameplayStatics::ApplyPointDamage(DamageActor,   //
                                           WeaponDamage,  //
                                           TraceStart,    //
                                           HitResult,     //
                                           Controller,    //
                                           Player,        //
                                           nullptr);      // TSubclassOf<UDamageType> DamageTypeClass
    }
    else
    {
        DrawDebugLine(GetWorld(),                //
                      GetMuzzleWorldLocation(),  //
                      TraceEnd,                  //
                      FColor::Purple,            //
                      false,                     //
                      5.f);                      //
    }

    DecreaseAmmo();
}
