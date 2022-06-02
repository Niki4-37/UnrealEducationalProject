// For educational purposes only.

#include "Weapon/EPProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

AEPProjectile::AEPProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SpherComponent");
    CollisionComponent->SetSphereRadius(10.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.f;
    MovementComponent->ProjectileGravityScale = 0.f;
}

void AEPProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(CollisionComponent);
    check(MovementComponent);
    MovementComponent->Velocity = ProjectileDirection * MovementComponent->InitialSpeed;

    CollisionComponent->OnComponentHit.AddDynamic(this, &AEPProjectile::OnComponentHit);

    SetLifeSpan(LifeTime);
}

void AEPProjectile::OnComponentHit(UPrimitiveComponent* HitComponent,  //
                                   AActor* OtherActor,                 //
                                   UPrimitiveComponent* OtherComp,     //
                                   FVector NormalImpulse,              //
                                   const FHitResult& Hit)              //
{
    if (!GetWorld()) return;

    UGameplayStatics::ApplyDamage(Hit.GetActor(),   //
                                  DamageAmount,     //
                                  GetController(),  //
                                  GetOwner(),       //
                                  DamageType);      //

    Destroy();
}

AController* AEPProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
