// For educational purposes only.


#include "UI/EPPickupInfoWidget.h"

void UEPPickupInfoWidget::ChangeTextColor()
{
    PlayAnimation(ChangeColor);
}

void UEPPickupInfoWidget::TurnBackTextColor() 
{
    PlayAnimationReverse(ChangeColor);
}
