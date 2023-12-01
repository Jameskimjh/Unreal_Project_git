

#include "Slash_Player.h"
#include "Animation/AnimMontage.h"
#include "Weapon.h"
#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "SlashHUD.h"
#include "SlashOverlay.h"
#include "AttributeComponent.h"
#include "UIControllerClass.h"
#include "BaseCharacter.h"
#include "HealthPotion.h"
#include "Slash_Anim.h"


ASlash_Player::ASlash_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 500.0f;

	Camera->SetupAttachment(springArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	springArm->bUsePawnControlRotation = true;
	springArm->bInheritPitch = true;
	springArm->bInheritRoll = true;
	springArm->bInheritYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Dash_Distance = 3500.0f;
	restoreHP = 20.0f;



}


void ASlash_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis  (FName("MoveForward"), this, &ASlash_Player::MoveForward);
	PlayerInputComponent->BindAxis  (FName("MoveRight"),   this, &ASlash_Player::MoveRight);
	PlayerInputComponent->BindAxis  (FName("LookUp"),      this, &ASlash_Player::LookUp);
	PlayerInputComponent->BindAxis  (FName("Turn"),        this, &ASlash_Player::Turn);

	PlayerInputComponent->BindAction(FName("Jump"),   IE_Pressed, this, &ASlash_Player::Jump); // 점프
	PlayerInputComponent->BindAction(FName("Equip"),  IE_Pressed, this, &ASlash_Player::RKeyPressed); //장비

	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlash_Player::Attack); //공격
	PlayerInputComponent->BindAction(FName("Dash"), IE_Pressed, this, &ASlash_Player::PlayRoll); // 대쉬
	PlayerInputComponent->BindAction(FName("Potion"), IE_Pressed, this, &ASlash_Player::UsingPotion); // 물약 사용
}

void ASlash_Player::Jump()
{
	if (IsUnoccupied()  )
	{
		Super::Jump();
	}

}


// TakeDamage : 공격 받을 때 받은 데미지 관리
float ASlash_Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	
	return DamageAmount;
}

// GetHite_Implementation : 공격을 받았을 때 발생하는 행동
void ASlash_Player::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) 
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.0f)
	{

		ActionState = EActionState::EAS_HitReaction;

	}
}

void ASlash_Player::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ASlash_Player::AddPotion(AHealthPotion* Potion)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddPotion(Potion->GetPotion());
		SlashOverlay->SetPotion(Attributes->GetPotion());
	}
}

void ASlash_Player::UsingPotion()
{
	if (Attributes->GetPotion() <= 0) return;

	if (Attributes && SlashOverlay)
	{

		Attributes->SubPotion();
		
		SlashOverlay->SetPotion(Attributes->GetPotion());
		
		Attributes->RestoreHP(restoreHP);
		
		SetHUDHealth(); // Slash_Player에 정의 되어있고 텍스트와 Health_Bar를 세팅해준다.

		ActionState = EActionState::EAS_Potion;

		PlayAnimMontage(CureMontage, 1.3f);
		
	}
}

void ASlash_Player::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngageableTarget"));
	InitializeSlashOverlay(); // WBP_PlayerOverlay HealthBar

	
}

void ASlash_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((Attributes->GetStamina() < 100.0f) && SlashOverlay && (ActionState != EActionState::EAS_Dead))
	{
		Attributes->HandleStamina(0.1f);
		SetHUDStamina();
	}
}

void ASlash_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}



//****************************************캐릭터의 전반적인 움직임들 ex) 앞으로 옆으로 뒤로 카메라의 이동****************************
//******************************************************************************************************************************
void ASlash_Player::MoveForward(float value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (value != 0.0f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);


	}
}

void ASlash_Player::MoveRight(float value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (value != 0.0f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);


	}

}

void ASlash_Player::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ASlash_Player::Turn(float value)
{
	AddControllerYawInput(value);

}
//**********************************************************************************************************************************
//**********************************************************************************************************************************


// ------------------------------------------- 무기에 Overlap됐을 때 클릭시 착용 이벤트 발생 -------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------

void ASlash_Player::RKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"),this,this );
		State = ECharacterState::ECS_EquippedOnehandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	
	else
	{
		if (CanDisarm())
		{
			Disarm();

		}
		
		else if (CanArm())
		{
			
			Arm();
		}
		
	}
}
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------



void ASlash_Player::Dash()
{
	
	if ((ActionState == EActionState::EAS_Unoccupied) && (Attributes->GetStamina() >= 30.0f))
	{
		UAnimInstance* SkillAnim = GetMesh()->GetAnimInstance();

		ActionState = EActionState::EAS_Dash;
		const FRotator Rotation = GetActorForwardVector().Rotation(); // 캐릭터 액터가 앞을 바라보는 방향
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		Anim->Montage_Play(SkillMontage);

		GetCharacterMovement()->BrakingFrictionFactor = 0.0f; // 마찰계수
		LaunchCharacter(Direction * Dash_Distance, true, true);
		
		Attributes->UseStamaina();
		GetWorldTimerManager().SetTimer(timerHandle, this, &ASlash_Player::StopDashing, 0.5f, false);

	}
	
}

void ASlash_Player::StopDashing()
{
	UAnimInstance* SkillAnim = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->StopMovementImmediately();
	Anim->Montage_Stop(0.15f,SkillMontage);

	GetWorldTimerManager().SetTimer(timerHandle, this, &ASlash_Player::ResetDash, 0.3f, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
}


void ASlash_Player::ResetDash()
{
	ActionState = EActionState::EAS_Unoccupied;

}





void ASlash_Player::Attack() // 공격 함수
{
	Super::Attack(); // BaseCharacter에서 선언된 함수를 오버라이딩 함

	/*if (CanAttack())
	{
		PlayAttackMontage();
	
		ActionState = EActionState::EAS_Attacking;
	}*/

	if (CanAttack() && !GetCharacterMovement()-> IsFalling())
	{
		ProcessComboCommand();
	}
	
	

} 

// 무기 장착 *************************************************
void ASlash_Player::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	State = ECharacterState::ECS_EquippedOnehandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}


// BlueprintCallable로 애니메이션 블루프린트에서 몽타주의 노티파이에 연결시킴
void ASlash_Player::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;

}

bool ASlash_Player::CanAttack()
{
	return  ActionState == EActionState::EAS_Unoccupied && 
		    State != ECharacterState::ECS_Unequipped;
	
}


void ASlash_Player::PlayRoll()
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	ActionState = EActionState::EAS_Dash;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Roll(RollMontage);
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * 3000.0f * GetWorld()->GetDeltaSeconds();
	SetActorLocation(NewLocation);

}




// 무기를 착용할 수 있는지 아닌지 확인하는 코드 + 장착 모션 + 장착******************************************
//*************************************************************************
bool ASlash_Player::CanDisarm()
{
	return 	ActionState == EActionState::EAS_Unoccupied 
		&& State != ECharacterState::ECS_Unequipped;
}

bool ASlash_Player::CanArm()
{
	return ActionState == EActionState   ::EAS_Unoccupied &&
		   State	   == ECharacterState::ECS_Unequipped &&
		   EquippedWeapon;
}

void ASlash_Player::Disarm()
{
	PlayEquipMontage(FName("UnEquip"));
	State = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlash_Player::Arm()
{
	PlayEquipMontage(FName("Equip"));
	State = ECharacterState::ECS_EquippedOnehandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlash_Player::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlash_Player::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlash_Player::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}

}
//*****************************************************************************
//*******************************************************************************

void ASlash_Player::Die()
{
	Super::Die();

	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();

	AUIControllerClass* PlayerController = Cast<AUIControllerClass>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->ShowRestartWidget();
	}
}

void ASlash_Player::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlash_Player::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;


}

void ASlash_Player::PotionEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}



void ASlash_Player::RollEnd()// AnimMontage 에 노티파이에 연결될 함수 (애니메이션 블루프린트 event 그래프 창에있음)
{
	Super::RollEnd();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//위의 코드는 Roll 할때 무적상태로 있으려고 한거야 그러니까 End일때는 당연히 전으로 돌아가게 된다.

	
	ActionState = EActionState::EAS_Unoccupied;	
}

bool ASlash_Player::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void ASlash_Player::InitializeSlashOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			 SlashOverlay = SlashHUD->GetSlashOverlay();

			if (SlashOverlay && Attributes)
			{
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetHealthText(Attributes->Get_Health(),Attributes->Get_MaxHealth());

			}

		}

	}
}


void ASlash_Player::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		

		SlashOverlay->SetHealthText(Attributes->Get_Health(),Attributes->Get_MaxHealth());
	}
}

void ASlash_Player::SetHUDStamina()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		SlashOverlay->SetStaminaText(Attributes->GetStamina(), Attributes->Get_MaxStamina());
	}
}







