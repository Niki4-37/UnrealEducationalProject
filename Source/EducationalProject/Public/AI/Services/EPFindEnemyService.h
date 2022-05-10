// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"

#include "EPFindEnemyService.generated.h"

UCLASS()
class EDUCATIONALPROJECT_API UEPFindEnemyService : public UBTService
{
	GENERATED_BODY()

public:
    UEPFindEnemyService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
    FBlackboardKeySelector EnemyActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

