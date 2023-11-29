// Fill out your copyright notice in the Description page of Project Settings.


#include "UIControllerClass.h"
#include "RestartWidget.h"

void AUIControllerClass::ShowRestartWidget()
{
	if (BP_RestartWidget != nullptr)
	{
		SetInputMode(FInputModeUIOnly());

		bShowMouseCursor = true;

		RestartWidget = CreateWidget<URestartWidget>(this, BP_RestartWidget);

		RestartWidget->AddToViewport();

	}
}

void AUIControllerClass::HideRestartWidget()
{
	RestartWidget->RemoveFromParent();
	RestartWidget->Destruct();


	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

}
