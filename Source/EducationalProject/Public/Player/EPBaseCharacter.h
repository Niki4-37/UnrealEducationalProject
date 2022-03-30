// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "Components/TimelineComponent.h"
#include "Components/InputComponent.h"

#include "Components/EPHealthComponent.h"
#include "Components/EPWeaponComponent.h"


#include "EPBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEPHealthComponent;
class UEPWeaponComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEPBaseCharacter(const FObjectInitializer& ObjInit);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
    UEPHealthComponent* HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UEPWeaponComponent* WeaponComponent;
    
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    
    /* Make Player moving faster*/
    bool bIsMoveForward = false;
    bool bWantsToRun = false;
    void StartRunning();
    void StopRunning();
};
