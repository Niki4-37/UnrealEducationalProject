// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDUCATIONALPROJECT_API UWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponFXComponent();

protected:
	virtual void BeginPlay() override;

};
