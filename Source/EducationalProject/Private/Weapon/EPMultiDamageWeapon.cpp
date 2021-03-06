// For educational purposes only.


#include "Weapon/EPMultiDamageWeapon.h"
#include "GameFramework/Character.h"
#include "Weapon/EPProjectile.h"

void AEPMultiDamageWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty()) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawningTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());

    auto Projectile = GetWorld()->SpawnActorDeferred<AEPProjectile>(ProjectileClass, SpawningTransform);
    if (Projectile)
    {
        Projectile->SetProjectileDirection(Direction);
        Projectile->FinishSpawning(SpawningTransform);
    }
    DecreaseAmmo();
}