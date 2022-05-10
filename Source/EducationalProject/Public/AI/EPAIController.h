// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EPAIController.generated.h"

class UEPAIPerceptionComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPAIController : public AAIController
{
	GENERATED_BODY()

public:
    AEPAIController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Component")
    UEPAIPerceptionComponent* EPAIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Behavior")
    FName FocusOnKeyName = "EnemyActor";
    
    virtual void OnPossess(APawn* InPawn) override;
    
    virtual void Tick(float DeltaTime) override;

private:
    AActor* FocusOnActor();

};
