// For educational purposes only.


#include "DamageFactors/EPDamegeActor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

AEPDamegeActor::AEPDamegeActor()
{
	PrimaryActorTick.bCanEverTick = true;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("DamageActor");
    SetRootComponent(SceneComponent);
}

void AEPDamegeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEPDamegeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 24, SphereColor);
    UGameplayStatics::ApplyRadialDamage(
        GetWorld(),
        Damage,
        GetActorLocation(),
        SphereRadius,
        DamageType,
        {},
        this,
        nullptr,
        bDoFoolDamage
    );
}

