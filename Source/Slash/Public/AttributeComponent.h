
#pragma once

#include"EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	// 현재 체력
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	// 전체 체력
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Maxhealth;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina ;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina ;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 PotionAmount;




public:

	void RestoreHP(float value);
	void ReceiveDamage(float Damage);
	void HandleStamina(float value);

	void UseStamaina(); //FKey 를 눌렀을 때 Stamina 25를 사용

	void RestoreHealth();
	bool IsAlive();

	void AddPotion(int32 potion);
	void SubPotion();


	// 체력 바********************
	float GetHealthPercent();
	float Get_Health();
	float Get_MaxHealth();
	//**********************

	// MP,Stamina등*******************
	float GetStaminaPercent();
	float GetStamina();
	float Get_MaxStamina();
	//*****************************
	
	FORCEINLINE int32 GetPotion() const{ return PotionAmount; }
	
};
