// For educational purposes only.

#include "Weapon/EPPistol.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraComponent.h" 
#include "NiagaraFunctionLibrary.h"

#include "AnimationHelper.h"
#include "Animation/EPShellCasingEjectAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(Pistol_LOG, All, All);

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

    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;

        DrawDebugSphere(GetWorld(),             //
                        HitResult.ImpactPoint,  //
                        5.f,                    //
                        24,                     //
                        FColor::Blue,           //
                        false,                  //
                        5.f);                   //

        const auto DamageActor = HitResult.GetActor();
        if (!DamageActor) return;

        UGameplayStatics::ApplyPointDamage(DamageActor,   //
                                           WeaponDamage,  //
                                           TraceStart,    //
                                           HitResult,     //
                                           Controller,    //
                                           Player,        //
                                           nullptr);      // TSubclassOf<UDamageType> DamageTypeClass
    }

    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
    DecreaseAmmo();
    
    Super::MakeShot();
}

void AEPPistol::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd) 
{
    UNiagaraComponent* const TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}

void AEPPistol::InitAnimation() 
{
    Super::InitAnimation();
    
    const auto ShellCasingEjectAnimNotify = AnimationHelper::FindNotifyByClass<UEPShellCasingEjectAnimNotify>(ShootingAnimation);
    if (ShellCasingEjectAnimNotify)
    {
        ShellCasingEjectAnimNotify->OnNotified.AddUObject(this, &AEPPistol::OnShellCasingEject);
    }
    else
    {
        UE_LOG(Pistol_LOG, Error, TEXT("Forgotten to set Shell casing eject notify"));
        checkNoEntry();
    }
}

void AEPPistol::OnShellCasingEject(USkeletalMeshComponent* MeshComponent)
{
    if (IsAmmoEmpty() && MeshComponent != WeaponMesh) return;
    SpawnEjectFX();
}
