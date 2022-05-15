// For educational purposes only.


#include "EPGameModeBase.h"
#include "Player/EPBaseCharacter.h"
#include "Player/EPPlayerController.h"
#include "UI/EPGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "AI/EPAICharacter.h"
#include "AI/EPSpawningActor.h"
#include "Components/EPHealthComponent.h"

AEPGameModeBase::AEPGameModeBase()
{
    DefaultPawnClass = AEPBaseCharacter::StaticClass();
    HUDClass = AEPGameHUD::StaticClass();
    PlayerControllerClass = AEPPlayerController::StaticClass();
}

void AEPGameModeBase::StartPlay()
{
    Super::StartPlay();

    GetAllSpawningActors();
    GetAllBots();
    StartSpawningProcess();
}

void AEPGameModeBase::StartSpawningProcess()
{
    GetWorldTimerManager().SetTimer(SpawningCountdown, this, &AEPGameModeBase::SpawnBot, RespawningDelay, true);
}

void AEPGameModeBase::GetAllSpawningActors()
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawningActorClass, SpawningActorsList);
}

void AEPGameModeBase::GetAllBots()
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), BotClass, BotsList);

    if (BotsList.Num() < BotsNum)
    {
        return;
    }
    GetWorldTimerManager().ClearTimer(SpawningCountdown);
}

void AEPGameModeBase::SpawnBot()
{
    if (SpawningActorsList.Num() == 0) return;
   
    const auto RandomSpawnerIndex = FMath::RandHelper(SpawningActorsList.Num() - 1);
    const auto RandomSpawningActor = Cast<AEPSpawningActor>(SpawningActorsList[RandomSpawnerIndex]);
    if (!RandomSpawningActor) return;
    RandomSpawningActor->SpawnBot();
    GetAllBots();
}

