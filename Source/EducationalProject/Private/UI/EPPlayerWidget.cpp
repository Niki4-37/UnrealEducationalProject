// For educational purposes only.


#include "UI/EPPlayerWidget.h"
#include "Components/EPHealthComponent.h"
#include "Components/EPWeaponComponent.h"

float UEPPlayerWidget::GetHealthPercent() const
{
    /*======== Put it in static template function ========*/
    //const auto Player = GetOwningPlayerPawn();
    //if (!Player) return 0.f;

    //const auto Component = Player->GetComponentByClass(UEPHealthComponent::StaticClass());
    /* no need to check nullptr, because Cast<>(nullptr) returns nullptr */
    //const auto HealthComponent = Cast<UEPHealthComponent>(Component);
    /*====================================================*/

    const auto HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<UEPHealthComponent>();
    //const auto HealthComponent = GetPlayerComponentByClass<UEPHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.f;

    return HealthComponent->GetHealthPercent();
}

bool UEPPlayerWidget::GetAmmoData(FAmmoData& Data) const
{
    const auto WeaponComponent = GetPlayerComponentByClass<UEPWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetAmmoData(Data);
}

bool UEPPlayerWidget::IsPlayerOverlaped() const
{
    return false;
}

FString UEPPlayerWidget::DecorateNumbers(int32 Numbers) const
{
    const int32 MaxLenght = 3;
    const TCHAR PrefixSymbol = '0';
    
    FString NumbersStr = FString::FromInt(Numbers);
    const auto SymbolsToAdd = MaxLenght - NumbersStr.Len();
    if ( SymbolsToAdd > 0)
    {
        NumbersStr = FString::ChrN(SymbolsToAdd, PrefixSymbol).Append(NumbersStr);
    }
    return NumbersStr;
}
