// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerController.h"
#include "Blueprint/UserWidget.h"

void AUI_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());

	

}
