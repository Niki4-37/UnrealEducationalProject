// For educational purposes only.

#include "AI/EPAIController.h"
#include "AI/EPAICharacter.h"
#include "Components/EPAIPerceptionComponent.h"
/* to get function GetBlackboardComponent() */
#include "BehaviorTree/BlackboardComponent.h"

AEPAIController::AEPAIController()
{
    /* replace standart PerceptionComponent by custom EPAIPerceptionComponent */
    EPAIPerceptionComponent = CreateDefaultSubobject<UEPAIPerceptionComponent>("EPAIPerceptionComponent");
    /* SetPerceptionComponent() tekes reference as a param */
    SetPerceptionComponent(*EPAIPerceptionComponent);
}

void AEPAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto EPAICharacter = Cast<AEPAICharacter>(InPawn);
    if (EPAICharacter)
    {
        RunBehaviorTree(EPAICharacter->BehaviorTreeAsset);
    }
}

void AEPAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto AimActor = FocusOnActor();
    SetFocus(AimActor);
}

AActor* AEPAIController::FocusOnActor()
{
    if (!GetBlackboardComponent()) return nullptr;
    /* Get pointer to Actor wich has added to blackboard with key "EnemyActor" */
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
