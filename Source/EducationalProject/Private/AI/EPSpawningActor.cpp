// For educational purposes only.


#include "AI/EPSpawningActor.h"
#include "AIController.h"
#include "EPGameModeBase.h"
#include "AI/EPAICharacter.h"

AEPSpawningActor::AEPSpawningActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEPSpawningActor::BeginPlay()
{
	Super::BeginPlay();

    check(GetWorld());
}

void AEPSpawningActor::SpawnBot()
{
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