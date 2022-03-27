// For educational purposes only.


#include "EPBaseCharacter.h"

AEPBaseCharacter::AEPBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SprengArm");
    SpringArm->SetupAttachment(RootComponent);
    
    /*  
    //This options better setup in Unreal Editor
    SpringArm->SocketOffset = FVector(0.f, 100.f, 100.f);
    SpringArm->bUsePawnControlRotation = true;
    */

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);

    WeaponComponent = CreateDefaultSubobject<UEPWeaponComponent>("WeaponComponent");
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

float AEPBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.f;

    const auto VelocityVector = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityVector));
    const auto CrossPoduct = FVector::CrossProduct(GetActorForwardVector(), VelocityVector);
    const auto Degreese = FMath::RadiansToDegrees(AngleBetween);

    return CrossPoduct.IsZero() ? Degreese : Degreese * FMath::Sign(CrossPoduct.Z);
}


