// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPPickupInfoWidget.generated.h"

UCLASS()
class EDUCATIONALPROJECT_API UEPPickupInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void ChangeTextColor();
    void TurnBackTextColor();

protected:
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ChangeColor;
};
