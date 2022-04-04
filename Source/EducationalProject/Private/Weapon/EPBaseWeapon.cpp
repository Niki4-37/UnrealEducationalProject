// For educational purposes only.


#include "Weapon/EPBaseWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"

AEPBaseWeapon::AEPBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
    
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void AEPBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
    check(WeaponMesh);
}

void AEPBaseWeapon::MakeShot()
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Player) return;
    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller) return;

    FVector VeiwLocation;
    FRotator VeiwRotation;
    Controller->GetPlayerViewPoint(VeiwLocation, VeiwRotation);
    
    const FVector BulletStart = WeaponMesh->GetSocketLocation(MuzzleSocketName);
    const FVector Direction = VeiwRotation.Vector();
    const FVector BulletFinish = BulletStart + Direction * BulletFlyMaxDistace;
    
    FCollisionQueryParams CollisionParams;

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, BulletStart, BulletFinish, ECollisionChannel::ECC_Visibility, CollisionParams);
    
    if (HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(), BulletStart, HitResult.ImpactPoint, FColor::Cyan, false, 1.f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 24, FColor::Blue, false, 2.f);
    }
    else
    {
        DrawDebugLine(GetWorld(), BulletStart, BulletFinish, FColor::Purple, false, 1.f);
    }
}

void AEPBaseWeapon::Fire()
{
    MakeShot();
}

