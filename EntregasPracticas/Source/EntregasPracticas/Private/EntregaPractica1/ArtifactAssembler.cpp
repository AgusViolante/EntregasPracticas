// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/EntregaPractica1/Items/FragmentItem.h"
#include "Public/EntregaPractica1/Components/FragmentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "public/EntregaPractica1/ArtifactAssembler.h"
#include "Public/EntregaPractica1/ArtifactAssembler.h"


// Sets default values
AArtifactAssembler::AArtifactAssembler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bIsReadyToActivate = false;
	
	TimeToAssemble = 3.0f;
	
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	RootComponent = VisualMesh;
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);

	InteractionSphere->SetSphereRadius(150.0f);

	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AArtifactAssembler::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundFragments;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFragmentItem::StaticClass(), FoundFragments);

	for (AActor* Actor : FoundFragments)
	{
		AFragmentItem* Fragment = Cast<AFragmentItem>(Actor);
		if (Fragment)
		{
			Fragment->OnFragmentCollected.AddDynamic(this, &AArtifactAssembler::OnFragmentCollectedCallback);
		}
	}
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("ArtifactAssembler: Suscrito a %d fragmentos."), FoundFragments.Num()));
	}
	UE_LOG(LogTemp, Log, TEXT("ArtifactAssembler: Suscrito a %d fragmentos."), FoundFragments.Num());

}

void AArtifactAssembler::OnFragmentCollectedCallback(FName FragmentID)
{
	if (ACharacter* MyPlayer = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		if (UFragmentComponent* FragComp = MyPlayer->FindComponentByClass<UFragmentComponent>())
		{
			FragComp->AddFragment(FragmentID);
			if (FragComp->IsArtifactComplete())
			{
				bIsReadyToActivate = true;
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,
						5.0f,
						FColor::Yellow,
						TEXT("ArtifactAssembler: Artefacto completo listo para interactuar.")
					);
				}
				UE_LOG(LogTemp, Warning, TEXT("ArtifactAssembler: Artefacto completo Listo para interactuar."));

			}
		}
	}
}

// Called every frame
void AArtifactAssembler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArtifactAssembler::Interact_Implementation(AActor* Interactor)
{
	if (bIsReadyToActivate && !GetWorldTimerManager().IsTimerActive(AssemblyTimerHandle))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.0f,
				FColor::Yellow,
				FString::Printf(TEXT("Ensamblaje iniciado... espera %f segundos."), TimeToAssemble)
			);
		}
		UE_LOG(LogTemp, Warning, TEXT("Ensamblaje iniciado... espera %f segundos."), TimeToAssemble);

		GetWorldTimerManager().SetTimer(
			AssemblyTimerHandle,
			this,
			&AArtifactAssembler::OnAssemblyFinished,
			TimeToAssemble,
			false
		);
	}
	else if (!bIsReadyToActivate)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No puedes interactuar"));
		UE_LOG(LogTemp, Error, TEXT("No podes interactuar: Faltan fragmentos."));
	}
}


void AArtifactAssembler::OnAssemblyFinished()
{
	if (RewardClass)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 100);
		FRotator SpawnRotation = GetActorRotation();

		GetWorld()->SpawnActor<AActor>(RewardClass, SpawnLocation, SpawnRotation);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.0f,
				FColor::Red,
				FString::Printf(TEXT("Se spawneo de vuelta")
			));
		}
		UE_LOG(LogTemp, Warning, TEXT("Se spawneo de vuelta"));
	}
	bIsReadyToActivate = false;
}

