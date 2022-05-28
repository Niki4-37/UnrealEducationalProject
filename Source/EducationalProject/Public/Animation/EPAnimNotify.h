// For educational purposes only.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EPAnimNotify.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
class EDUCATIONALPROJECT_API UEPAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
    virtual void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;

    FOnNotifiedSignature OnNotified;
};
