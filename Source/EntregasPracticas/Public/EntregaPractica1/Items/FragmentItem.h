

#pragma once

#include "CoreMinimal.h"
#include "Public/EntregaPractica1/Items/ItemBase.h"
#include "FragmentItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFragmentCollectedSignature, FName, FragmentID);

UCLASS()
class ENTREGASPRACTICAS_API AFragmentItem : public AItemBase
{
	GENERATED_BODY()
	
	
public:
	AFragmentItem();
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnFragmentCollectedSignature OnFragmentCollected;
	
	virtual void Interact_Implementation(AActor* Interactor) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="FragmentData")
	FName FragmentID;
};
