// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
/* for FAmmoData */
#include "Weapon/EPBaseWeapon.h"

#include "EPPlayerWidget.generated.h"



UCLASS()
class EDUCATIONALPROJECT_API UEPPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable)
    bool GetAmmoData(FAmmoData& Data) const;

private:
    template<typename T>
    static T* GetPlayerComponentByClass(AActor* SomeActor)
    {
        if (!SomeActor) return nullptr;

        const auto Component = SomeActor->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }

    UFUNCTION(BlueprintCallable)
    FString DecorateNumbers (int32 Numbers) const;
};
