
#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "CharacterType.h"

#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;



UCLASS()
class SLASH_API ABaseCharacter : public ACharacter , public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	

protected:


	virtual void BeginPlay() override;


	//*************Hitinterface**************************************************************
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void GetSkill_Implementation() override;
	//***************************************************************************************
	
	
	virtual void Attack();
	virtual void Die();
	
	virtual void Roll(UAnimMontage* Montage);
	
	void DirectionalHitReact(const FVector& ImpactPoint); // 공격 받을때 방향
	
	// ******************데미지를 다루는 virtual 함수 선언********************************
	virtual void HandleDamage(float DamageAmount);
	//*************************************************************************
	
	void PlayHitSound(const FVector& ImpackPoint);			//부딪혔을때 발생하는 소리
	void SpawnHitParticles(const FVector& ImpactPoint);		// 파티클 스폰


	void DisableCapsule();
	virtual bool CanAttack();

	bool IsAlive();
	void DisableMeshCollision();
	

	void PlayHitReactMontage(const FName& SectionName);

	virtual int32 PlayAttackMontage(); // 공격시 몽타주 실행

	virtual int32 PlayDeathMontage();


	virtual void PlayDodgeMontage();

	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
	virtual void RollEnd(); // 구르기 끝날때
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd(); // 공격 끝날 때
	
	// ***************************** 콤보공격 함수**********************************
	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();
	//*****************************************************************************

	//********************************* 콤보공격 변수******************************
	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

	//****************************************************************************
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
	

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = Combat)
	double WarpTargetDistance = 60.0f;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	
	UPROPERTY(EditDefaultsOnly, Category = Roll)
	UAnimMontage* RollMontage;
	
	//************************ 
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage; // 공격 몽타주
	//****************************

	// Enemy가 Hit됐을때 발생하는 몽타주**********************************************
	UPROPERTY(EditDefaultsOnly,Category = Combat)
	UAnimMontage* SkillHitReactMontage;
	//******************************************************************************


	//******** 플레이어 콤보 공격 몽타주 ****************************************
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackData, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboData> ComboActionData;
	//***********************************************************
	
private:

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	
	
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	

	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound; // 소리

	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DeathMontage;


	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	
	 
public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
	// enum 타입을 안전하게 다룰 수 있도록 도와주는 템플릿 클래스 
	// 정수 값인 열거형의 명시성을 강화 + 오류 방지

protected:

	
};
