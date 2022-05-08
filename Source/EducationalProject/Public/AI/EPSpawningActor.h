// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPSpawningActor.generated.h"

class AAIController;

UCLASS()
class EDUCATIONALPROJECT_API AEPSpawningActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPSpawningActor();

    void SpawnBot();

protected:
	virtual void BeginPlay() override;
    
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> PawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;
};
