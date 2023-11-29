// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIControllerClass.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API AUIControllerClass : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URestartWidget> BP_RestartWidget;

	void ShowRestartWidget();
	void HideRestartWidget();

private:
	UPROPERTY()
	class URestartWidget* RestartWidget;
};
