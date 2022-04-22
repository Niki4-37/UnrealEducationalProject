// For educational purposes only.


#include "AI/EPAIController.h"
#include "AI/EPAICharacter.h"


void AEPAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto EPAICharacter = Cast<AEPAICharacter>(InPawn);
    if (EPAICharacter)
    {
        RunBehaviorTree(EPAICharacter->BehaviorTreeAsset);
    }
}
