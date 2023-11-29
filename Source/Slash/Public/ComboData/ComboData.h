// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboData.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UComboData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UComboData();

	UPROPERTY(EditAnywhere,Category = Name)
	FString MontageSection;

	UPROPERTY(EditAnywhere,Category = Name)
	uint8 MaxCombo;

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate; //재생속도
	
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;


};
