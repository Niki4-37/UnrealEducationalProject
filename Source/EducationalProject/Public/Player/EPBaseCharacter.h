// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "Components/TimelineComponent.h"

#include "EPBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEPBaseCharacter();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* Camera;
    
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    void MoveForward(float Amount);
    void MoveRight(float Amount);

};
