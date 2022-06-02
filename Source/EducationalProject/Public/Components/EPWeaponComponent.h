// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPCoreData.h"

#include "EPWeaponComponent.generated.h"

class AEPBaseWeapon;
class UAnimMontage;
class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FWaeponAnimData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    TSubclassOf<AEPBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* EquipAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* ReloadAnimation;

    //TArray<UAnimSequence*> AnimSequences;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EDUCATIONALPROJECT_API UEPWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEPWeaponComponent();

    void Fire();
    /* use this info in widget*/
    bool GetAmmoData(FAmmoData& Data) const;

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

    int32 CurrentWeaponIndex;

    bool bEquipAnimInProgress = false;
    bool bReloadAnimInProgress = false;
    
    /*template <typename RequiredClass, typename UserClass>
    void BindFuncToNotifyEvent(UAnimMontage* Animation, UserClass* InUserObject, ????)
    {
        if (!Animation || !InUserObject) return;
        auto AnimNotify = FindNotifyByClass<RequiredClass>(Animation);
        if (!AnimNotify) return;
        AnimNotify->OnNotified.AddUObject(InUserObject, ????);
    }*/

    void InitAnimation();
    void PlayAnimation(UAnimMontage* Animation);
    void IntoTheHolster(USkeletalMeshComponent* MeshComponent);
    void FromTheHolster(USkeletalMeshComponent* MeshComponent);
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    void SpawnWeapon();
    void AttachWeaponToSocket(AEPBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);

    bool CanEquip();
    bool CanReload();

    template <typename Y>
    Y* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation) return nullptr;
        
        const TArray<FAnimNotifyEvent> Notifies = Animation->Notifies;
        for (auto& NotifyEvent : Notifies)
        {
            auto RequiredNotify = Cast<Y>(NotifyEvent.Notify);
            if (RequiredNotify) return RequiredNotify;
        }
        return nullptr;
    }
};
