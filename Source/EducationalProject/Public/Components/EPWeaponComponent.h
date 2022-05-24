// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/EPBaseWeapon.h"

#include "EPWeaponComponent.generated.h"

class AEPBaseWeapon;

USTRUCT(BlueprintType)
struct FWaeponAnimData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    TSubclassOf<AEPBaseWeapon> WeaponClass;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
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
    //UPROPERTY()
    //TArray<UAnimSequence*> CurrentAnimationSequences;

    UPROPERTY()
    TArray<AEPBaseWeapon*> OwningWeapons;

    UPROPERTY()
    AEPBaseWeapon* CurrentWeapon = nullptr;

    int32 CurrentWeaponIndex;
    
    void SpawnWeapon();
    void AttachWeaponToSocket(AEPBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);
};
