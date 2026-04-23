// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/EntregaPractica1/Components/FragmentComponent.h"

// Sets default values for this component's properties
UFragmentComponent::UFragmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	TotalFragmentNeeded = 3;

	// ...
}


// Called when the game starts
void UFragmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFragmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UFragmentComponent::AddFragment(FName FragmentID)
{
	if (!FragmentID.IsNone() && !CollectedFragments.Contains(FragmentID))
	{
		CollectedFragments.Add(FragmentID);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Green,
				FString::Printf(
					TEXT("Fragmento recolectado: %s. Total: %d/%d"),
					*FragmentID.ToString(),
					CollectedFragments.Num(),
					TotalFragmentNeeded
				)
			);
		}

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Fragmento recolectado: %s. Total: %d/%d"),
			*FragmentID.ToString(),
			CollectedFragments.Num(),
			TotalFragmentNeeded
		);
	}
}

bool UFragmentComponent::IsArtifactComplete() const
{
	return CollectedFragments.Num() >= TotalFragmentNeeded;
}

