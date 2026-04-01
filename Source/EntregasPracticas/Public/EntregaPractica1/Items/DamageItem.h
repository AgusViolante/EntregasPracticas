// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/EntregaPractica1/Items/ItemBase.h"
#include "DamageItem.generated.h"

/**
 * 
 */
UCLASS()
class ENTREGASPRACTICAS_API ADamageItem : public AItemBase
{
	GENERATED_BODY()

public:
	ADamageItem();
	virtual void Interact_Implementation(AActor* Interactor) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="DamageData")
	float DamageAmount;
};
