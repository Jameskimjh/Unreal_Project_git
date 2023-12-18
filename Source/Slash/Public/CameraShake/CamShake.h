// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "CamShake.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UCamShake : public UCameraShakeBase
{
	GENERATED_BODY()
	
public:

	void CameraShake();

protected:
	//virtual NativeCameraShake(float Deltatime,)
	
};
