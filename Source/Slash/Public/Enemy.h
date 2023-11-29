
#pragma once

#include "EngineMinimal.h"
#include "BaseCharacter.h"
#include "CharacterType.h"
#include "Enemy.generated.h"


class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy(); 
	//<AActor>
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	//<AActor>
	
	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	// <IHitInterface>

protected:
	//AActor
	virtual void BeginPlay() override;
	//AActor

	//ABaseCharacter
	virtual void Die() override;
	virtual void Attack() override; // 공격 
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	//ABaseCharacter


	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;



private:
	// AI_Behavior
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for Onpawnseen in  UPawnSeenComponent

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> Weaponclass;


	UPROPERTY(EditAnywhere)
	double CombatRadius = 350.0f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 100.0f;

	UPROPERTY()
	class AAIController* EnemyController;

	// 현재 patrol Target;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.0f;

	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.0f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.0f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.0f;

	UPROPERTY(EditAnywhere , Category = Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.0f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.0f;




};
