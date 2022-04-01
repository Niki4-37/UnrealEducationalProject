// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EPGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class EDUCATIONALPROJECT_API AEPGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UserInerface")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	
    virtual void BeginPlay() override;
};
