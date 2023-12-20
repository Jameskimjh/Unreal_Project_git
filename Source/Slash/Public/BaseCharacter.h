
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
	
	void DirectionalHitReact(const FVector& ImpactPoint); // ���� ������ ����
	
	// ******************�������� �ٷ�� virtual �Լ� ����********************************
	virtual void HandleDamage(float DamageAmount);
	//*************************************************************************
	
	void PlayHitSound(const FVector& ImpackPoint);			//�ε������� �߻��ϴ� �Ҹ�
	void SpawnHitParticles(const FVector& ImpactPoint);		// ��ƼŬ ����


	void DisableCapsule();
	virtual bool CanAttack();

	bool IsAlive();
	void DisableMeshCollision();
	

	void PlayHitReactMontage(const FName& SectionName);

	virtual int32 PlayAttackMontage(); // ���ݽ� ��Ÿ�� ����

	virtual int32 PlayDeathMontage();


	virtual void PlayDodgeMontage();

	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
	virtual void RollEnd(); // ������ ������
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd(); // ���� ���� ��
	
	// ***************************** �޺����� �Լ�**********************************
	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();
	//*****************************************************************************

	//********************************* �޺����� ����******************************
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
	UAnimMontage* AttackMontage; // ���� ��Ÿ��
	//****************************

	// Enemy�� Hit������ �߻��ϴ� ��Ÿ��**********************************************
	UPROPERTY(EditDefaultsOnly,Category = Combat)
	UAnimMontage* SkillHitReactMontage;
	//******************************************************************************


	//******** �÷��̾� �޺� ���� ��Ÿ�� ****************************************
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackData, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboData> ComboActionData;
	//***********************************************************
	
private:

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	
	
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	

	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound; // �Ҹ�

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
	// enum Ÿ���� �����ϰ� �ٷ� �� �ֵ��� �����ִ� ���ø� Ŭ���� 
	// ���� ���� �������� ��ü��� ��ȭ + ���� ����

protected:

	
};
