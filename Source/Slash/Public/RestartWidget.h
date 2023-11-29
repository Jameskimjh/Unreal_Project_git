// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartWidget.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API URestartWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	class UButton* RestartButton;

protected:
	UFUNCTION()
	void OnRestartClicked();
	
	virtual void NativeOnInitialized() override;


	
};
