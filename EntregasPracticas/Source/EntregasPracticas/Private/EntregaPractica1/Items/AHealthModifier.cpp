// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "EntregaPractica1/Items/AHealthModifier.h"
#include "EntregaPractica1//Items/AHealthModifier.h"
#include "EntregasPracticasCharacter.h"
#include "Engine/DamageEvents.h"


// Sets default values
AAHealthModifier::AAHealthModifier()
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneComponent"));
	RootComponent = ZoneComponent;

	ZoneComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZoneComponent->SetCollisionObjectType(ECC_WorldDynamic);
	ZoneComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ZoneComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ZoneComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AAHealthModifier::BeginPlay()
{
	Super::BeginPlay();
	
	ZoneComponent->OnComponentBeginOverlap.AddDynamic(this, &AAHealthModifier::OnZoneBeginOverlap);
	ZoneComponent->OnComponentEndOverlap.AddDynamic(this, &AAHealthModifier::OnZoneEndOverlap);
}

void AAHealthModifier::OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEntregasPracticasCharacter* Character = Cast<AEntregasPracticasCharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	OverlappingCharacter = Character;
	StartEffectTimer();
}

void AAHealthModifier::OnZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OverlappingCharacter)
	{
		OverlappingCharacter = nullptr;
		StopEffectTimer();
	}
}

void AAHealthModifier::StartEffectTimer()
{
	if (!GetWorld())
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(EffectTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		EffectTimerHandle,
		this,
		&AAHealthModifier::ApplyEffect,
		TickInterval,
		true
	);
}

void AAHealthModifier::StopEffectTimer()
{
	if (!GetWorld())
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(EffectTimerHandle);
}

void AAHealthModifier::ApplyEffect()
{
	if (!OverlappingCharacter)
	{
		StopEffectTimer();
		return;
	}

	if (bUseDamage)
	{
		OverlappingCharacter->TakeDamage(
			EffectAmount,
			FDamageEvent(),
			nullptr,
			this
		);
	}
	else
	{
		OverlappingCharacter->ApplyHeal(EffectAmount);
	}
	OnHealthModifierTick.Broadcast(bUseDamage, EffectAmount);
}

// Called every frame
void AAHealthModifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

