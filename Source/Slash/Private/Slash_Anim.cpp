// Fill out your copyright notice in the Description page of Project Settings.


#include "Slash_Anim.h"
#include "Slash_Player.h"
#include "Kismet/KismetMathLibrary.h"

void USlash_Anim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	slashCharacter = Cast<ASlash_Player>(TryGetPawnOwner());
	if (slashCharacter)
	{
		slashCharacterMovement = slashCharacter->GetCharacterMovement();
	}


}

void USlash_Anim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (slashCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(slashCharacterMovement->Velocity);
		IsFalling = slashCharacterMovement->IsFalling();
		CharacterState = slashCharacter->GetCharacterState();
		ActionState = slashCharacter->GetActionState();
		DeathPose = slashCharacter->GetDeathPose();
	}
}



