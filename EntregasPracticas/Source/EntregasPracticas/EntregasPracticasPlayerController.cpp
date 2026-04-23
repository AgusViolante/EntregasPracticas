#include "EntregasPracticasPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EntregasPracticas.h"
#include "EntregasPracticasCharacter.h"
#include "EntregaPractica1/UI/HealthWidget.h"
#include "InputMappingContext.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AEntregasPracticasPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (HealthWidgetClass)
		{
			HealthWidget = CreateWidget<UHealthWidget>(this, HealthWidgetClass);
			if (HealthWidget)
			{
				HealthWidget->AddToPlayerScreen(10);
			}
			else
			{
				UE_LOG(LogEntregasPracticas, Error, TEXT("No se pudo crear HealthWidget."));
			}
		}

		BindToCharacter();
	}
}

void AEntregasPracticasPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	BindToCharacter();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				if (CurrentContext)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}

			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					if (CurrentContext)
					{
						Subsystem->AddMappingContext(CurrentContext, 0);
					}
				}
			}
		}
	}
}

void AEntregasPracticasPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	BindToCharacter();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				if (CurrentContext)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}

			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					if (CurrentContext)
					{
						Subsystem->AddMappingContext(CurrentContext, 0);
					}
				}
			}
		}
	}
}

void AEntregasPracticasPlayerController::BindToCharacter()
{
	if (CachedCharacter)
	{
		CachedCharacter->OnHealthChanged.RemoveDynamic(this, &AEntregasPracticasPlayerController::HandleHealthChanged);
	}

	CachedCharacter = Cast<AEntregasPracticasCharacter>(GetPawn());

	if (CachedCharacter)
	{
		CachedCharacter->OnHealthChanged.AddDynamic(this, &AEntregasPracticasPlayerController::HandleHealthChanged);
		HandleHealthChanged(CachedCharacter->GetCurrentHealth(), CachedCharacter->GetMaxHealth());
	}
}

void AEntregasPracticasPlayerController::HandleHealthChanged(float CurrentHealth, float MaxHealth)
{
	if (HealthWidget)
	{
		HealthWidget->UpdateHealthUI(CurrentHealth, MaxHealth);
	}
}

void AEntregasPracticasPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				if (CurrentContext)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}

			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					if (CurrentContext)
					{
						Subsystem->AddMappingContext(CurrentContext, 0);
					}
				}
			}
		}
	}
}

bool AEntregasPracticasPlayerController::ShouldUseTouchControls() const
{
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}