// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}

}

void USlashOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressbar)
	{
		StaminaProgressbar->SetPercent(Percent);
	}
}

void USlashOverlay::SetHealthText(float health,float maxhealth)
{
	const FString String = FString::Printf(TEXT("%3.f/%3.f"), health,maxhealth);
	const FText Text = FText::FromString(String);
	HealthText->SetText(Text);
}

void USlashOverlay::SetStaminaText(float Stamina, float maxStamina)
{
	const FString String = FString::Printf(TEXT("%3.f/%3.f"), Stamina, maxStamina);
	const FText Text = FText::FromString(String);
	StaminaText->SetText(Text);
}

void USlashOverlay::SetPotion(int32 potion)
{
	if (PotionAmount)
	{
		const FString Potion_String = FString::Printf(TEXT("%d"), potion);
		const FText Text = FText::FromString(Potion_String);
		PotionAmount->SetText(Text);
	}
}




