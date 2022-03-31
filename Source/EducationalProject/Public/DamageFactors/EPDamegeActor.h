// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPDamegeActor.generated.h"

UCLASS()
class EDUCATIONALPROJECT_API AEPDamegeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPDamegeActor();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USceneComponent* SceneComponent;
    
    /* Set current damage type in Unreal Editor */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float SphereRadius = 200.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FColor SphereColor = FColor::Blue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float Damage = 1.f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool bDoFoolDamage = true;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
