// For educational purposes only.


#include "UI/EPPlayerWidget.h"
#include "Components/EPHealthComponent.h"

float UEPPlayerWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return 0.f;

    const auto Component = Player->GetComponentByClass(UEPHealthComponent::StaticClass());
    /* no need to check nullptr, because Cast<>(nullptr) returns nullptr */
    const auto HealthComponent = Cast<UEPHealthComponent>(Component);
    if (!HealthComponent) return 0.f;

    return HealthComponent->GetHealthPercent();
}
