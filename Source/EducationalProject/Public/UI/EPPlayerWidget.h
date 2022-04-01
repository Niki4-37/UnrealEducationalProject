// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPPlayerWidget.generated.h"


UCLASS()
class EDUCATIONALPROJECT_API UEPPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const;
};
