// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/EntregaPractica1/InteractableInterface.h"
#include "ArtifactAssembler.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class ENTREGASPRACTICAS_API AArtifactAssembler : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArtifactAssembler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* VisualMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsReadyToActivate;

	UFUNCTION()
	void OnFragmentCollectedCallback(FName FragmentID);

	FTimerHandle AssemblyTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Assembly")
	float TimeToAssemble;

	UPROPERTY(EditAnywhere, Category = "Assembly")
	TSubclassOf<AActor> RewardClass;

	void OnAssemblyFinished();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
	virtual void Interact_Implementation(AActor* Interactor) override;


};
