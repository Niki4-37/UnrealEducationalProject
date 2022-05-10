// For educational purposes only.


#include "AI/Services/EPFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/EPAIPerceptionComponent.h"
/* for local variable Controller with type AAIController */
#include "AIController.h"

UEPFindEnemyService::UEPFindEnemyService()
{
    NodeName = "Find Enemy";
}

void UEPFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = Controller->FindComponentByClass<UEPAIPerceptionComponent>();
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
        }
    }
        
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
