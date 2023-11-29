// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Intro_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API AIntro_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable,Category = "UMG_Game")
	void ChangeWidget(TSubclassOf<UUserWidget> widgetclass);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "UMG_Game")
	TSubclassOf<UUserWidget> StartWidget;

	UPROPERTY()
	UUserWidget* CurrentWidget;

};
