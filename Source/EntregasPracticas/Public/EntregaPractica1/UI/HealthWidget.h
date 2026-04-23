#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UProgressBar;

UCLASS()
class ENTREGASPRACTICAS_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

public:
	UFUNCTION()
	void UpdateHealthUI(float CurrentHealth, float MaxHealth);
};