// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EPAIController.generated.h"

UCLASS()
class EDUCATIONALPROJECT_API AEPAIController : public AAIController
{
	GENERATED_BODY()

protected:
    virtual void OnPossess(APawn* InPawn) override;
};
