// For educational purposes only.


#include "EPGameModeBase.h"
#include "Player/EPBaseCharacter.h"
#include "UI/EPGameHUD.h"

AEPGameModeBase::AEPGameModeBase()
{
    DefaultPawnClass = AEPBaseCharacter::StaticClass();
    HUDClass = AEPGameHUD::StaticClass();
}