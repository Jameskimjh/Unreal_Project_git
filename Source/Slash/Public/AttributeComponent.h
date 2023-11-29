
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
	// ���� ü��
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	// ��ü ü��
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

	void UseStamaina(); //FKey �� ������ �� Stamina 25�� ���

	void RestoreHealth();
	bool IsAlive();

	void AddPotion(int32 potion);
	void SubPotion();


	// ü�� ��********************
	float GetHealthPercent();
	float Get_Health();
	float Get_MaxHealth();
	//**********************

	// MP,Stamina��*******************
	float GetStaminaPercent();
	float GetStamina();
	float Get_MaxStamina();
	//*****************************
	
	FORCEINLINE int32 GetPotion() const{ return PotionAmount; }
	
};
