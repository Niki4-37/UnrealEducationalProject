// For educational purposes only.

#include "DamageFactors/EPDamageFactor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AEPDamageFactor::AEPDamageFactor()
{
    PrimaryActorTick.bCanEverTick = true;

    DamageFactor = CreateDefaultSubobject<USceneComponent>("DamageFactor");
    SetRootComponent(DamageFactor);
}

void AEPDamageFactor::BeginPlay()
{
    Super::BeginPlay();
}

void AEPDamageFactor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetWorld())
    {
        DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, DebugSphereColor);
        
        UGameplayStatics::ApplyRadialDamage(GetWorld(),          //
                                            DealDamage,          //
                                            GetActorLocation(),  //
                                            DamageRadius,        //
                                            DamageType,          //
                                            {},                  //
                                            this,                //
                                            nullptr,             //
                                            bDoFullDamage);      //
    }
}
