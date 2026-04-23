// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EntregasPracticasPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UHealthWidget;
class AEntregasPracticasCharacter;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AEntregasPracticasPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** UI de vida */
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UHealthWidget> HealthWidgetClass;

	/** UI instanciada */
	UPROPERTY()
	TObjectPtr<UHealthWidget> HealthWidget;

	/** Character cacheado */
	UPROPERTY()
	TObjectPtr<AEntregasPracticasCharacter> CachedCharacter;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** If true, the player will use UMG touch controls even if not playing on mobile platforms */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	/** Llamado cuando posee un pawn */
	virtual void OnPossess(APawn* InPawn) override;

	/** Llamado en cliente cuando replica el pawn */
	virtual void OnRep_Pawn() override;

	/** Returns true if the player should use UMG touch controls */
	bool ShouldUseTouchControls() const;

	UFUNCTION()
	void HandleHealthChanged(float CurrentHealth, float MaxHealth);

	void BindToCharacter();
};