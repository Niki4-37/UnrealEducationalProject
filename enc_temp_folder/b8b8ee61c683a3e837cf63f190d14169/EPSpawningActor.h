// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPSpawningActor.generated.h"

class AAIController;
class USphereComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPSpawningActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPSpawningActor();

    void SpawnBot();

    bool CanSpawn() const { return bIsSpawnReady; };

protected:
	virtual void BeginPlay() override;
    
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> PawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* ActorCollisionComponent;

private:
    bool bIsSpawnReady;
    
    UFUNCTION()
    void OnActorCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnActorCollisionComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
