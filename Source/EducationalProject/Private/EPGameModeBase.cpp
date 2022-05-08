// For educational purposes only.


#include "EPGameModeBase.h"
#include "Player/EPBaseCharacter.h"
#include "UI/EPGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "AI/EPAICharacter.h"
#include "AI/EPSpawningActor.h"
#include "Components/EPHealthComponent.h"

AEPGameModeBase::AEPGameModeBase()
{
    DefaultPawnClass = AEPBaseCharacter::StaticClass();
    HUDClass = AEPGameHUD::StaticClass();
}

void AEPGameModeBase::StartPlay()
{
    Super::StartPlay();

    GetAllSpawningActors();
    GetAllBots();
    
}

void AEPGameModeBase::GetAllSpawningActors()
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawningActorClass, SpawningActorsList);
}

void AEPGameModeBase::GetAllBots()
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), BotClass, BotsList);

    GetWorldTimerManager().SetTimer(SpawningCountdown, this, &AEPGameModeBase::SpawnBot, RespawningTime, true);
    
}

void AEPGameModeBase::SpawnBot()
{
    const auto RandomSpawnerIndex = FMath::RandHelper(SpawningActorsList.Num());
    const auto RandomSpawningActor = Cast<AEPSpawningActor>(SpawningActorsList[RandomSpawnerIndex]);
    if (!RandomSpawningActor) return;
    RandomSpawningActor->SpawnBot();
    GetAllBots();
    if (BotsList.Num() >= BotsNum)
    {
        //GetWorldTimerManager().PauseTimer(SpawningCountdown);
        GetWorldTimerManager().ClearTimer(SpawningCountdown);
        
        /*for (const auto Bot : BotsList)
        {
            const auto EPAICharacter = Cast<AEPAICharacter>(Bot);
            if (!EPAICharacter) continue;
            const auto Component = EPAICharacter->GetComponentByClass(UEPHealthComponent::StaticClass());
            if (!Component) continue;
            const auto HealthComponent = Cast<UEPHealthComponent>(Component);
            if (HealthComponent && !HealthComponent->OnDeath.IsBoundToObject(this))
            {
                HealthComponent->OnDeath.AddUObject(this, &AEPGameModeBase::OnDeath);
            }
        }*/
    }
}

void AEPGameModeBase::OnDeath(FVector ShotFromDirection, FName BoneName)
{
    //UE_LOG(LogTemp, Warning, TEXT("Death happend"));
    //GetAllBots();
    GetWorldTimerManager().UnPauseTimer(SpawningCountdown);
    //SpawnBot();
}
