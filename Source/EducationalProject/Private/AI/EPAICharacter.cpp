// For educational purposes only.


#include "AI/EPAICharacter.h"
#include "AI/EPAIController.h"
#include "Components/EPHealthComponent.h"
/* for GetCharacterMovement() */
#include "GameFramework/CharacterMovementComponent.h"
/* use BrainComponent's functions */
#include "BrainComponent.h"

AEPAICharacter::AEPAICharacter()
{
	HealthComponent = CreateDefaultSubobject<UEPHealthComponent>("HealthComponent");
    PrimaryActorTick.bCanEverTick = false;
    
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; //Disabled
    AIControllerClass = AEPAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = false;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
    }
}

void AEPAICharacter::BeginPlay()
{
	Super::BeginPlay();

    check (GetMesh());
    HealthComponent->OnDeath.AddUObject(this, &AEPAICharacter::OnDeath);
}

void AEPAICharacter::OnDeath(FVector ShotFromDirection, FName BoneName)
{
    GetCharacterMovement()->DisableMovement();
    /* Add phisics, makes Plyer ragdoll */
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true); 
    /* find base class controller */
    const auto AIController = Cast<AAIController>(Controller);
    /* check pointer and BrainComponent (#include "BrainComponent.h") */
    if (AIController && AIController->BrainComponent)
    {
        /* stop behavior tree */
        AIController->BrainComponent->Cleanup();
    }

    GetMesh()->AddForce(ShotFromDirection * 500, BoneName);
    /* destroy actor after given time */
    SetLifeSpan(5.f);   //destroy after death and make change with staticmesh
}

