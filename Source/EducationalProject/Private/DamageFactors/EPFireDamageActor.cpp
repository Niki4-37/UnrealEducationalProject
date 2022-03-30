// For educational purposes only.


#include "DamageFactors/EPFireDamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AEPFireDamageActor::AEPFireDamageActor()
{
	PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("FireDamage");
    SetRootComponent(SceneComponent);

}

void AEPFireDamageActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEPFireDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 24, SphereColor);
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), SphereRadius, nullptr, {}, this, nullptr, bDoFoolDamage);
}

