// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "Components/TimelineComponent.h"

#include "Weapon/EPBaseWeapon.h"

#include "EPBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AEPBaseWeapon;

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

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AEPBaseWeapon> PlayerWeapon;
    
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    UFUNCTION(BlueprintCallable)
    float GetMovementDirection() const;

private:
    void SpawnWeapon();

};
