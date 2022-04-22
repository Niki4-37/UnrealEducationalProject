// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EPAICharacter.generated.h"

class UBehaviorTree;
class UEPHealthComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEPAICharacter();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
    UEPHealthComponent* HealthComponent;

private:
    /* Function binded to HealthComponent FOnDeath delegate*/
    void OnDeath(FVector ShotFromDirection, FName BoneName);

};
