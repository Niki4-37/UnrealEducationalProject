// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "EPAIPerceptionComponent.generated.h"

class AEPPlayerController;

UCLASS()
class EDUCATIONALPROJECT_API UEPAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
    AActor* GetClosestEnemy() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Behavior")
    TSubclassOf<AEPPlayerController> ControllerType;
};
