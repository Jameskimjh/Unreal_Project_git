

#include "Enemy.h"
#include "AIController.h"
#include "AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"



AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);


	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.0f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage"));
	

	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;

	}
	else if (IsOutsideAttackRadius())
	{
		ChaseTarget();
	}


	return DamageAmount;
}




void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}


void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);

	if (!IsDead())
	{
		ShowHealthBar();
	}
	ClearPatrolTimer(); // ���ݹް� �����̴°� ��������
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	StopAttackMontage();

}

void AEnemy::GetSkill_Implementation()
{
	Super::GetSkill_Implementation();

	if (!IsDead())
	{
		ShowHealthBar();
	}

	ClearPatrolTimer(); // ���ݹް� �����̴°� ��������
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	StopAttackMontage();
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
	InitializeEnemy();
	Tags.Add(FName("Enemy"));
	
}

void AEnemy::Die()
{
	Super::Die();
	EnemyState = EEnemyState::EES_Dead;
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	DisableMeshCollision();
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsEngaged() &&
		!IsAttacking() &&
		!IsDead();

	return bCanAttack;
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}

}



void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}

void AEnemy::CheckPatrolTarget()
{

	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float randomTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, randomTime);
	}
}

void AEnemy::CheckCombatTarget() // ���� ã���� �Լ�
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();

		if (!IsEngaged())
		{
			StartPatrolling();
			Attributes->RestoreHealth();
		}
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged())
		{
			ChaseTarget();
		}
	}

	else if (CanAttack())
	{
		StartAttackTimer();
	}
}



void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
	
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest() // ���� ���� ��ħ
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling() // Target Point �� ��ȸ��
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget() // ���� ����
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);

}

bool AEnemy::IsOutsideCombatRadius() //�ν� ���� �ۿ� ����
{
	return !InTargetRange(CombatTarget,CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius() // ���ݹ��� �ۿ� ������
{
	return !InTargetRange(CombatTarget,AttackRadius);
}

bool AEnemy::IsInsideAttackRadius() // ���� ���� �ȿ� ����
{
	return InTargetRange(CombatTarget,AttackRadius);
}

bool AEnemy::IsChasing() // ���� ����
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking() // ���� �� ����
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead() // �������
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged() // ���� � �ൿ�� ���ϴ� ����
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);

}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(55.0f);
	EnemyController->MoveTo(MoveRequest);
	

}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	
	for (auto Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target); // TArray�Լ��� �̹� �ش� ��Ұ� �����ϴ��� Ȯ����

		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return  ValidTargets[TargetSelection];
		
	}
	return nullptr;
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && Weaponclass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(Weaponclass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}


void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking && // 
		SeenPawn->ActorHasTag(FName("EngageableTarget"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
	
}





