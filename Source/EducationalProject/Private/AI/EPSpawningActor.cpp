// For educational purposes only.

#include "AI/EPSpawningActor.h"
#include "AIController.h"
#include "EPGameModeBase.h"
#include "AI/EPAICharacter.h"
#include "Components/SphereComponent.h"

AEPSpawningActor::AEPSpawningActor()
{
    PrimaryActorTick.bCanEverTick = false;

    ActorCollisionComponent = CreateDefaultSubobject<USphereComponent>("ActorCollisionComponent");

    ActorCollisionComponent->SetupAttachment(RootComponent);
    ActorCollisionComponent->SetSphereRadius(100.f);
    ActorCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    ActorCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEPSpawningActor::BeginPlay()
{
    Super::BeginPlay();

    check(GetWorld());
    check(ActorCollisionComponent);

    ActorCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEPSpawningActor::OnActorCollisionComponentBeginOverlap);
    ActorCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AEPSpawningActor::OnActorCollisionComponentEndOverlap);

}

void AEPSpawningActor::OnActorCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,  //
                                                             AActor* OtherActor,                        //
                                                             UPrimitiveComponent* OtherComp,            //
                                                             int32 OtherBodyIndex,                      //
                                                             bool bFromSweep,                           //
                                                             const FHitResult& SweepResult)             //
{
    bIsSpawnOccupied = ActorCollisionComponent->IsOverlappingActor(OtherActor);
    UE_LOG(LogTemp, Display, TEXT("Actor: %s"), *GetDebugName(OtherActor));
}

void AEPSpawningActor::OnActorCollisionComponentEndOverlap(UPrimitiveComponent* OverlappedComponent,  //
                                                           AActor* OtherActor,                        //
                                                           UPrimitiveComponent* OtherComp,            //
                                                           int32 OtherBodyIndex)                      //
{
    bIsSpawnOccupied = ActorCollisionComponent->IsOverlappingActor(OtherActor);
}

void AEPSpawningActor::SpawnBot()
{
    FString TextReady = bIsSpawnOccupied ? "true" : "false";
    UE_LOG(LogTemp, Display, TEXT("Actor: %s, SpawnOccupied: %s"), *GetDebugName(this), *TextReady);
    
    //if (!GetWorld() || bIsSpawnOccupied) return;
    if (!GetWorld()) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const auto NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, GetActorLocation(), GetActorRotation(), SpawnParams);
    if (!NewPawn) return;

    if (!NewPawn->Controller)
    {
        NewPawn->SpawnDefaultController();
    }

    const auto AIController = Cast<AAIController>(NewPawn->Controller);
    if (AIController)
    {
        const auto AICharacter = Cast<AEPAICharacter>(AIController->GetCharacter());
        if (!AICharacter) return;

        AIController->RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }
}
