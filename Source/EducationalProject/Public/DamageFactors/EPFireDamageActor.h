// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPFireDamageActor.generated.h"

class UEPFireDamageType;

UCLASS()
class EDUCATIONALPROJECT_API AEPFireDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPFireDamageActor();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USceneComponent* SceneComponent;

    float SphereRadius = 50.f;
    FColor SphereColor = FColor::Cyan;
    float Damage = 1.f;
    bool bDoFoolDamage = true;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
