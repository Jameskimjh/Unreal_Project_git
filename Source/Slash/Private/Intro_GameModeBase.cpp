// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro_GameModeBase.h"
#include <Blueprint/UserWidget.h>

void AIntro_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ChangeWidget(StartWidget);
}


void AIntro_GameModeBase::ChangeWidget(TSubclassOf<UUserWidget> widgetclass)
{
	if (CurrentWidget != nullptr) 
	{
		CurrentWidget = nullptr;
	}
	if (widgetclass != nullptr)
	{
		CurrentWidget = CreateWidget(GetWorld(), widgetclass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}