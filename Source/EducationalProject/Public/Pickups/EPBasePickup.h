// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPBasePickup.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS()
class EDUCATIONALPROJECT_API AEPBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPBasePickup();

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent* CollisionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* InfoWidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    float VisibilityDistace = 1000.f;

	virtual void BeginPlay() override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor);
    virtual void NotifyActorEndOverlap(AActor* OtherActor);

    virtual bool GivePickup(APawn* Pawn);

    template <typename Y>
    Y* GetRequiredComponent(AActor* Actor)
    {
        if (!Actor) return nullptr;

        const auto Component = Actor->GetComponentByClass(Y::StaticClass());
        return Cast<Y>(Component);
    }

 private:
    void UpdateInfoWidgetVisibility();

    template<typename Y>
    Y* GetWidgetFromComponent(UWidgetComponent* WidgetComponent)
    {
        const auto Widget = WidgetComponent->GetWidget();
        return Cast<Y>(Widget);
    }
};
