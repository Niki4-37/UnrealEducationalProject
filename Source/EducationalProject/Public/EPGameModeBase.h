// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "EPGameModeBase.generated.h"

//class AEPSpawningActor;
class AEPAICharacter;

UCLASS()
class EDUCATIONALPROJECT_API AEPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    AEPGameModeBase();

    virtual void StartPlay() override;

    //int32 GetBotsNum() const { return BotsNum; }

    void StartSpawningProcess();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    int32 BotsNum = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "0", ClampMax = "10"))
    float RespawningDelay = 2.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
    TSubclassOf<AActor> SpawningActorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
    TSubclassOf<ACharacter> BotClass;

private:
    bool bCanSpawn = true;
    FTimerHandle SpawningCountdown;

    TArray<AActor*> SpawningActorsList;

    TArray<AActor*> BotsList;
    
    void GetAllSpawningActors();

    void GetAllBots();

    void SpawnBot();

    
};
