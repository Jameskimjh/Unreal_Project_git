// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "UIControllerClass.h"


void URestartWidget::OnRestartClicked()
{
	AUIControllerClass* PlayerController = Cast<AUIControllerClass>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideRestartWidget();
	}
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void URestartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (RestartButton != nullptr)
	{
		RestartButton->OnClicked.AddDynamic(this, &URestartWidget::OnRestartClicked);

	}
}
