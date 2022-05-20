// For educational purposes only.

#include "Components/EPCharacterMovementComponent.h"
#include "Player/EPBaseCharacter.h"  //Makes error Circular Dependencies in compiler

float UEPCharacterMovementComponent::GetMaxSpeed() const
{
    /* Get MaxSpeed from parent Character's class settings */
    const float MaxSpeed = Super::GetMaxSpeed();
    /* Get pointer to Player to check running motion. Cast to character because
     *  CharacterMovementComponent has pointer only for Pawn */
    const auto Player = Cast<AEPBaseCharacter>(GetPawnOwner());
    /* If Player Runing so modify MaxSpeed or stay previouse from settings */
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
