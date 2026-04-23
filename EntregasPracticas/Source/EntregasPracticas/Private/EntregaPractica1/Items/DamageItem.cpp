// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/EntregaPractica1/Items/DamageItem.h"
#include "Public/EntregaPractica1/InteractableInterface.h"
#include  "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ADamageItem::ADamageItem()
{
	DamageAmount= 10.0f;
}

void ADamageItem::Interact_Implementation(AActor* Interactor)
{
	Super::Interact_Implementation(Interactor);
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter, DamageAmount, nullptr, this, UDamageType::StaticClass());
	}
	
	Destroy();
}
