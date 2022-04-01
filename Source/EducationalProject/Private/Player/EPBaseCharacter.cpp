// For educational purposes only.


#include "EPBaseCharacter.h"
#include "Components/EPCharacterMovementComponent.h"
#include "Engine/Engine.h"

/* Using this type of Constructor to change Default CharacterMovementComponent to Custom class*/
AEPBaseCharacter::AEPBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UEPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SprengArm");
    SpringArm->SetupAttachment(GetRootComponent());
    
    /*  
    //This options better setup in Unreal Editor
    SpringArm->SocketOffset = FVector(0.f, 100.f, 100.f);
    SpringArm->bUsePawnControlRotation = true;
    */

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);

    HealthComponent = CreateDefaultSubobject<UEPHealthComponent>("HealthComponent");

    WeaponComponent = CreateDefaultSubobject<UEPWeaponComponent>("WeaponComponent");
}

void AEPBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(HealthComponent);
    check(GetMesh());
    /* Get Delegate from Health Component and bind function. AddUObject binds only C++ delegate */
    HealthComponent->OnDeath.AddUObject(this, &AEPBaseCharacter::OnDeath);
}

void AEPBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


void AEPBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AEPBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AEPBaseCharacter::MoveRight);
    /* Mouse veiw */
    PlayerInputComponent->BindAxis("LookUp", this, &AEPBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &AEPBaseCharacter::AddControllerYawInput);
    /* Make character move faster */
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AEPBaseCharacter::StartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AEPBaseCharacter::StopRunning);

}
/* Use this function in animation blueprint to set direction in statemachine */
float AEPBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.f;

    const auto VelocityVector = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityVector));
    const auto CrossPoduct = FVector::CrossProduct(GetActorForwardVector(), VelocityVector);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossPoduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossPoduct.Z);
}

bool AEPBaseCharacter::IsRunning() const
{
    /* By checking velocity exclude character in variouse trap or stuck in textures */
    return bWantsToRun && bIsMoveForward && !GetVelocity().IsZero();
}

void AEPBaseCharacter::MoveForward(float Amount)
{
    if (FMath::IsNearlyZero(Amount)) return;
    bIsMoveForward = Amount > 0.f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AEPBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void AEPBaseCharacter::StartRunning()
{
    bWantsToRun = true;
}

void AEPBaseCharacter::StopRunning()
{
    bWantsToRun = false;
}
/* Function binded to HealthComponent FOnDeath delegate*/
void AEPBaseCharacter::OnDeath()
{
    GetCharacterMovement()->DisableMovement();
    /* Add phisics, makes Plyer ragdoll*/
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

