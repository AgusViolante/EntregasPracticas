// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FragmentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENTREGASPRACTICAS_API UFragmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFragmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FragmentData")
	int32 TotalFragmentNeeded;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FragmentData")
	TArray<FName> CollectedFragments;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Artifact")
	void AddFragment(FName FragmentID);

	UFUNCTION(BlueprintPure, Category = "Artifact")
	bool IsArtifactComplete() const;

	int32 GetCollectedCount() const { return CollectedFragments.Num(); }

		
};
