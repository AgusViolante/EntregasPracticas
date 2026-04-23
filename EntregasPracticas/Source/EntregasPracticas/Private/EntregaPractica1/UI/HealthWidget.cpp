#include "EntregaPractica1/UI/HealthWidget.h"
#include "Components/ProgressBar.h"


void UHealthWidget::UpdateHealthUI(float CurrentHealth, float MaxHealth)
{
	if (HealthProgressBar && MaxHealth > 0.f)
	{
		float HealthPercentage = CurrentHealth / MaxHealth;
		HealthProgressBar->SetPercent(HealthPercentage);
	}
}