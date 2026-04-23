// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/EntregaPractica1/Items/FragmentItem.h"

AFragmentItem::AFragmentItem()
{
	FragmentID = FName();
}

void AFragmentItem::Interact_Implementation(AActor* Interactor)
{
	Super::Interact_Implementation(Interactor);
	OnFragmentCollected.Broadcast(FragmentID);
	Destroy();
}
