// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EPBaseWeapon.h"

#include "EPPistol.generated.h"

class UNiagaraSystem;

UCLASS()
class EDUCATIONALPROJECT_API AEPPistol : public AEPBaseWeapon
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* TraceFX;

    /* bind settings with Niagara System in Unreal Editor using user's variable */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    virtual void MakeShot() override;
    
private:
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

    virtual void InitAnimation() override;
    void OnShellCasingEject(USkeletalMeshComponent* MeshComponent);
};
