
#pragma once

#include "EngineMinimal.h"
#include "BaseCharacter.h"
#include "Item.h"
#include "Weapon.h"
#include "CharacterType.h"
#include "PickUpInterface.h"
#include "Slash_Player.generated.h"

class AItem;
class UAnimMontage;
class USlashOverlay;
class AHealthPotion;
class USlash_Anim;

UCLASS()
class SLASH_API ASlash_Player : public ABaseCharacter,public IPickUpInterface
{
	GENERATED_BODY()

public:
	ASlash_Player();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(class AItem* Item);
	virtual void AddPotion(AHealthPotion* Potion) override;


	UPROPERTY(EditAnywhere)
	float Dash_Distance;

	UPROPERTY()
	FTimerHandle timerHandle;

	
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;


protected:

	//callback for input
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);

	
	void RKeyPressed();
	void UsingPotion();
	
	
	virtual void Attack() override;
	
	//Combat
	void EquipWeapon(AWeapon* Weapon);

	virtual void AttackEnd() override;
	virtual bool CanAttack() override;

	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName);
	virtual void Die() override;

	void PlayRoll();
	virtual void RollEnd() override;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	void PotionEnd();
	// ½ºÅ³***************************
	UFUNCTION(BlueprintCallable)
	void BurstSkill();

	void Skill_Check(TArray<FHitResult>& HitResults);

	UFUNCTION(BlueprintCallable)
	void SkillEnd();

	void PlaySkill();

	bool CanSkill(UAnimInstance* AnimInstance);


private:
	bool IsUnoccupied();
	void InitializeSlashOverlay();
	void SetHUDHealth();
	void SetHUDStamina();
	
	void ExecuteGetSkill(const FHitResult& HitResult);

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArm;

	UPROPERTY(BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY();
	float stamina;
	
	float restoreHP;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float Skill_Radius;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;
	
	ECharacterState State = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	USlashOverlay* SlashOverlay;

	UPROPERTY()
	USlash_Anim* Anim;

	UPROPERTY(EditDefaultsOnly,Category = Montages)
	UAnimMontage* CureMontage;

	UPROPERTY(EditDefaultsOnly,Category = "Skill")
	UAnimMontage* BurstSkill_Montage;



	UPROPERTY(EditDefaultsOnly, Category = "SKill")
	TObjectPtr<class UAnimMontage> SkillMontage;


	

	UPROPERTY(EditAnywhere,Category = "Skill")
	UParticleSystem* SkillParticle;
	


public:
	FORCEINLINE ECharacterState GetCharacterState() const{ return State; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	
	
};
