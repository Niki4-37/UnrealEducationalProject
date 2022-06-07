// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPCoreData.h"

#include "EPWeaponComponent.generated.h"

class AEPBaseWeapon;
class UAnimMontage;
class USkeletalMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EDUCATIONALPROJECT_API UEPWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEPWeaponComponent();

    void Fire();
    /* use this info in widget*/
    bool GetAmmoData(FAmmoData& Data) const;
    /* Used in AnimationBlueprint */
    UFUNCTION(BlueprintCallable)
    AEPBaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; };

    void NextWeapon();

    void Reload();

protected:
    /* New array with weapons */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TArray<FWaeponAnimData> WeaponAnimData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName WeaponArmedSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY()
    TArray<AEPBaseWeapon*> OwningWeapons;

    UPROPERTY()
    AEPBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    UAnimMontage* CurrentEquipAnimation = nullptr;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimation = nullptr;

    UPROPERTY()
    UAnimMontage* CurrentSingleFireAnimation = nullptr;

    int32 CurrentWeaponIndex;

    bool bEquipAnimInProgress = false;
    bool bReloadAnimInProgress = false;

    void InitAnimation();
    void PlayAnimation(UAnimMontage* Animation);
    void IntoTheHolster(USkeletalMeshComponent* MeshComponent);
    void FromTheHolster(USkeletalMeshComponent* MeshComponent);
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
    
    void OnShellCasingEject(USkeletalMeshComponent* MeshComponent);

    void SpawnWeapon();
    void AttachWeaponToSocket(AEPBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);

    bool CanEquip();
    bool CanReload();
    bool CanDoAction();

    void OnClipEmpty();
    void ChangeClip();
};
