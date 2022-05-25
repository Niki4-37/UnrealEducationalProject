// For educational purposes only.


#include "Weapon/EPProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AEPProjectile::AEPProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SpherComponent");
    CollisionComponent->SetSphereRadius(10.f);
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
    SetLifeSpan(5.f);
	
}
