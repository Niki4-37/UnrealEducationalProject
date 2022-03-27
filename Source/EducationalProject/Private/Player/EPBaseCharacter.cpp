// For educational purposes only.


#include "EPBaseCharacter.h"

AEPBaseCharacter::AEPBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SprengArm");
    SpringArm->SetupAttachment(RootComponent);
    
    /*  
    //This options better setup in Unreal Editor
    SpringArm->SocketOffset = FVector(0.f, 100.f, 50.f);
    SpringArm->bUsePawnControlRotation = true;
    */

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);
}

void AEPBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AEPBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AEPBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AEPBaseCharacter::MoveRight);
    
    PlayerInputComponent->BindAxis("LookUp", this, &AEPBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &AEPBaseCharacter::AddControllerYawInput);

}

void AEPBaseCharacter::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AEPBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}


