// Copyright Epic Games, Inc. All Rights Reserved.

#include "EntregasPracticasCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "Public/EntregaPractica1/InteractableInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SphereComponent.h"
#include "InputActionValue.h"
#include "EntregasPracticas.h"
#include "EntregaPractica1/Items/AHealthModifier.h"
#include "Kismet/GameplayStatics.h"
#include "Public/EntregaPractica1/Components/FragmentComponent.h"

class AAHealthModifier;

AEntregasPracticasCharacter::AEntregasPracticasCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->InitSphereRadius(150.f);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionObjectType(ECC_Pawn);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	
	FragmentComp = CreateDefaultSubobject<UFragmentComponent>(TEXT("FragmentComponent"));
}

void AEntregasPracticasCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundModifiers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAHealthModifier::StaticClass(), FoundModifiers);

	for (AActor* Actor : FoundModifiers)
	{
		if (AAHealthModifier* Modifier = Cast<AAHealthModifier>(Actor))
		{
			Modifier->OnHealthModifierTick.AddDynamic(this, &AEntregasPracticasCharacter::OnHealthModifierTickReceived);
		}
	}
}

void AEntregasPracticasCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEntregasPracticasCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AEntregasPracticasCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEntregasPracticasCharacter::Look);
		
		EnhancedInputComponent->BindAction(InteractableAction, ETriggerEvent::Started, this, &AEntregasPracticasCharacter::PerformInteraction);
	}
	else
	{
		UE_LOG(LogEntregasPracticas, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEntregasPracticasCharacter::PerformInteraction()
{
	if (!InteractionSphere) return;

	TArray<AActor*> OverlappingActors;

	InteractionSphere-> GetOverlappingActors(OverlappingActors);


	for (AActor* HitActor : OverlappingActors)
	{

		if (HitActor && HitActor->Implements<UInteractableInterface>())
		{

			IInteractableInterface::Execute_Interact(HitActor, this);


			break;
		}
	}
}

void AEntregasPracticasCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AEntregasPracticasCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AEntregasPracticasCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AEntregasPracticasCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AEntregasPracticasCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AEntregasPracticasCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AEntregasPracticasCharacter::ApplyHeal(float HealAmount)
{
	if (HealAmount <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Green,
			FString::Printf(TEXT("Curado: %.2f | Vida actual: %.2f | Ticks curación: %d"),
				HealAmount, CurrentHealth, HealTicksReceived)
		);
	}
}

float AEntregasPracticasCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount <= 0.0f)
	{
		return 0.0f;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Red,
			FString::Printf(TEXT("Daño recibido: %.2f | Vida actual: %.2f | Ticks daño: %d"),
				DamageAmount, CurrentHealth, DamageTicksReceived)
		);
	}

	return DamageAmount;
}

void AEntregasPracticasCharacter::OnHealthModifierTickReceived(bool bUseDamage, float Amount)
{
	if (bUseDamage)
	{
		DamageTicksReceived++;
	}
	else
	{
		HealTicksReceived++;
	}
}
