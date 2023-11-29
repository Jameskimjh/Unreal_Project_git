// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Animation/AnimInstance.h"
#include "Slash_Player.h"
#include "Slash_Anim.generated.h"


/**
 * 
 */
UCLASS()
class SLASH_API USlash_Anim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ASlash_Player* slashCharacter;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* slashCharacterMovement;


	UPROPERTY(BlueprintReadOnly,Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly,Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly,Category = Movement)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathPose> DeathPose;
	

	UPROPERTY(EditAnywhere,Category = Movement)
	bool IsDash;



};
