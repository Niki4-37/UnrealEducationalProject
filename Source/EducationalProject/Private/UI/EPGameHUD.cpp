// For educational purposes only.

#include "UI/EPGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

void AEPGameHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrosshair();
}

void AEPGameHUD::BeginPlay()
{
    Super::BeginPlay();

    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}

void AEPGameHUD::DrawCrosshair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    const float HalfLineSize{10.f};
    const float LineThikness{2.f};
    const FColor LineColor{FColor::Cyan};

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThikness);
    DrawLine(Center.Min, Center.Max - HalfLineSize , Center.Min, Center.Max + HalfLineSize, LineColor, LineThikness);
}
