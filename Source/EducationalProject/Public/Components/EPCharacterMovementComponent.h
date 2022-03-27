// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EPCharacterMovementComponent.generated.h"


UCLASS()
class EDUCATIONALPROJECT_API UEPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", ClampMax = "5.0"))
    float RunModifier = 2.f;

    virtual float GetMaxSpeed() const override;
};
