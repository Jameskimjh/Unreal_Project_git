

#include "BaseCharacter.h"
#include "Weapon.h"
#include "AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ComboData/ComboData.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}

	else
	{
		Die();
	}

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}



void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
}

void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();

}

void ABaseCharacter::Roll(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && Montage)
	{
		PlayMontageSection(RollMontage, FName("Default"));

	}
}


void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();

	const FVector ImpactLowerd(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * Cos(theta) , 벡터의 내적을 나타냄
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// 아크 코사인 함수를 통해 을 통해 각도를 구함 
	double Theta = FMath::Acos(CosTheta);

	//라디안을 각도로 변환
	Theta = FMath::RadiansToDegrees(Theta);

	// 벡터의 외적을 구함
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.0f && Theta < 45.0f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.0f && Theta < -45.0f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.0f && Theta < 135.0f)
	{
		Section = FName("FromRight");

	}

	PlayHitReactMontage(Section);

}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}

}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}

}



void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);

	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{// 랜덤공격
	if (SectionNames.Num() <= 0) return -1;
	
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	
	PlayMontageSection(Montage, SectionNames[Selection]); // 섹션별로 실행
	
	return Selection;
}

//*************************************************** 콤보공격 *********************************************************************************
//*******************************************************************************************************************************************
void ABaseCharacter::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid()) //ComboTimerHandle의 유효성 판단
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void ABaseCharacter::ComboActionBegin()
{
	//콤보시작
	CurrentCombo = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage);
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABaseCharacter::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate,AttackMontage);
	
	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ABaseCharacter::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperty)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}

void ABaseCharacter::SetComboCheckTimer()
{
	
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.3f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ABaseCharacter::ComboCheck, ComboEffectiveTime, false);
	}
	
}

void ABaseCharacter::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxCombo);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSection, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, AttackMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;

	}

}

//********************************************************************************************************************************************
//********************************************************************************************************************************************



int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);

}



int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);

	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose; // 랜덤이라서 enum class 처럼 명시적으로 해줄 필요 없음
	}

	return Selection;
}


void ABaseCharacter::PlayDodgeMontage()
{
	PlayMontageSection(RollMontage, FName("Roll"));
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f,AttackMontage);

	}
}

//*************************************************Base Enemy 블루프린트 안에 이벤트 그래프 창 안에 구현 되어있음**************************************************************
//*************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************
FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if(CombatTarget == nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}
//*************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************


//************************** slash Player 안에 오버라이딩 되어있음 ******************************************************************************************************
//*************************************************************************************************************************************************************************

void ABaseCharacter::RollEnd()
{
	
	

}


// 공격 끝났을 때 
void ABaseCharacter::AttackEnd()
{

}

//*************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************


void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}



void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();

	}
}

