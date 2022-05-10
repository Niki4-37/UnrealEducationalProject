// For educational purposes only.

#include "Components/EPAIPerceptionComponent.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "Player/EPPlayerController.h"

AActor* UEPAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercievedActorsList;

    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercievedActorsList);
    if (PercievedActorsList.Num() == 0 ) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = FLT_MAX;
    AActor* BestPawn = nullptr;

    for (const auto& PercievedActor : PercievedActorsList)
    {
        const auto PercievedPawn = Cast<APawn>(PercievedActor);
        const auto PawnController = PercievedPawn->GetController();
        if (PawnController->IsA(ControllerType))
            BestPawn = PercievedActor;
    }

    return BestPawn;
}
