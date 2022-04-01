// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPDamageFactor.generated.h"

UCLASS()
class EDUCATIONALPROJECT_API AEPDamageFactor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPDamageFactor();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USceneComponent* DamageFactor;
    /* Set current damage type in Unreal Editor */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageType")
    float DamageRadius = 100.f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageType")
    FColor DebugSphereColor = FColor::Cyan;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageType")
    float DealDamage = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageType")
    bool bDoFullDamage = true;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
