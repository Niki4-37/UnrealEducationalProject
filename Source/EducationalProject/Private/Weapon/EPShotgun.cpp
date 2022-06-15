// For educational purposes only.


#include "Weapon/EPShotgun.h"
#include "Weapon/EPProjectile.h"

void AEPShotgun::MakeShot() 
{
    if (!GetWorld()) return;

    FVector VeiwLocation;
    FRotator VeiwRotation;
    GetPlayerVeiwpoint(VeiwLocation, VeiwRotation);

    const auto PojectilesNum = FMath::RandHelper(6) + 6;

    for (int8 Index = 0; Index < PojectilesNum; ++Index)
    {
        const auto HalfAngleSpreadRad = FMath::DegreesToRadians(ProjectileSpread);
        const FVector ShotDirection = FMath::VRandCone(VeiwRotation.Vector(), HalfAngleSpreadRad);

        const FTransform SpawningTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());

        auto Projectile = GetWorld()->SpawnActorDeferred<AEPProjectile>(ProjectileClass, SpawningTransform);
        if (Projectile)
        {
            Projectile->SetProjectileDirection(ShotDirection);
            Projectile->SetActorEnableCollision(false);
            Projectile->FinishSpawning(SpawningTransform);
        }
    }
}
