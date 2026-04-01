// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AHealthModifier.generated.h"

class UBoxComponent;
class AEntregasPracticasCharacter;

UCLASS()
class ENTREGASPRACTICAS_API AAHealthModifier : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAHealthModifier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* ZoneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Modifier")
	bool bUseDamage = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Modifier", meta = (ClampMin = "0.0"))
	float EffectAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Modifier", meta = (ClampMin = "0.1"))
	float TickInterval = 1.0f;
	
	
private:
	FTimerHandle EffectTimerHandle;
	
	UPROPERTY() AEntregasPracticasCharacter* OverlappingCharacter = nullptr;
	
	UFUNCTION()
	void OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void StartEffectTimer();
	void StopEffectTimer();
	void ApplyEffect();
	void Tick(float DeltaTime);
};