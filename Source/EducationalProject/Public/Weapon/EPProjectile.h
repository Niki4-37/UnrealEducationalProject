// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPProjectile.generated.h"

UCLASS()
class EDUCATIONALPROJECT_API AEPProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPProjectile();

protected:
	virtual void BeginPlay() override;
};
