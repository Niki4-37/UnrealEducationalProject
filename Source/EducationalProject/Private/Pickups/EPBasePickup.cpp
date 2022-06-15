// For educational purposes only.

#include "Pickups/EPBasePickup.h"
#include "Components/SphereComponent.h"
#include "Player/EPBaseCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "UI/EPPickupInfoWidget.h"

#include "Components/WidgetComponent.h"
#include "UI/EPPickupInfoWidget.h"

DEFINE_LOG_CATEGORY_STATIC(BasePickup_LOG, All, All);

AEPBasePickup::AEPBasePickup()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionCompnent");
    CollisionComponent->SetSphereRadius(80.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);

    /* Set current widget class in blueprint */
    InfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("InfoWidgetComponent");
    InfoWidgetComponent->SetupAttachment(CollisionComponent);
    InfoWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    //InfoWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    InfoWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    InfoWidgetComponent->SetCollisionProfileName("NoCollision");
    InfoWidgetComponent->SetDrawAtDesiredSize(true);
    InfoWidgetComponent->SetTwoSided(true);
}

void AEPBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateInfoWidgetVisibility();
}

void AEPBasePickup::BeginPlay()
{
    Super::BeginPlay();

    check(CollisionComponent);
    check(InfoWidgetComponent);
}

void AEPBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto EPInfoWidget = GetWidgetFromComponent<UEPPickupInfoWidget>(InfoWidgetComponent);
    if (OtherActor->IsA<AEPBaseCharacter>() && EPInfoWidget)
    {
        /* Play animation changing color */
        EPInfoWidget->ChangeTextColor();
    }
    UE_LOG(BasePickup_LOG, Display, TEXT("%s overlaped"), *OtherActor->GetName());

    const auto PlayerPawn = Cast<APawn>(OtherActor);
    GivePickup(PlayerPawn);
}

void AEPBasePickup::NotifyActorEndOverlap(AActor* OtherActor) 
{
    UE_LOG(BasePickup_LOG, Display, TEXT("%s end overlap"), *OtherActor->GetName());
    const auto EPInfoWidget = GetWidgetFromComponent<UEPPickupInfoWidget>(InfoWidgetComponent);
    if (OtherActor->IsA<AEPBaseCharacter>() && EPInfoWidget)
    {
        /* Play animation changing color */
        EPInfoWidget->TurnBackTextColor();
    }
    Super::NotifyActorEndOverlap(OtherActor);
}

void AEPBasePickup::UpdateInfoWidgetVisibility()
{
    if (!GetWorld() ||                                                  //
        !GetWorld()->GetFirstPlayerController() ||                      //
        !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())  //
        return;

    /* Alternative way to get Pawn position
     * APawn* OurPawn = UGameplayStatics::GetPlayerPawn(this, 0);
     * FVector PawnLocation = OurPawn->GetActorLocation();
     */
    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    InfoWidgetComponent->SetVisibility(Distance < VisibilityDistace, true);
}

bool AEPBasePickup::GivePickup(APawn* Pawn)
{
    return false;
}


