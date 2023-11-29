// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeComponent.h"
#include "SlashOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	void SetHealthText(float health,float maxhealth);
	void SetStaminaText(float Stamina, float maxStamina);
	void SetPotion(int32 potion);
private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaProgressbar;


	UPROPERTY(EditAnywhere)
	UAttributeComponent* attributes;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StaminaText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PotionAmount;
};
