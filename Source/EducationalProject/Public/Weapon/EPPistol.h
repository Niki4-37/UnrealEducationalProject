// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EPBaseWeapon.h"

#include "EPPistol.generated.h"

UCLASS()
class EDUCATIONALPROJECT_API AEPPistol : public AEPBaseWeapon
{
	GENERATED_BODY()

protected:
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    virtual void MakeShot() override;
    
};
