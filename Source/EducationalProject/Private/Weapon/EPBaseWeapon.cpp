// For educational purposes only.


#include "Weapon/EPBaseWeapon.h"

AEPBaseWeapon::AEPBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
    
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void AEPBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEPBaseWeapon::Fire()
{
    
}

