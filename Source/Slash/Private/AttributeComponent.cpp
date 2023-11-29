

#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PotionAmount = 0;
}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, Maxhealth);
}

void UAttributeComponent::RestoreHP(float value)
{
	Health = FMath::Clamp(Health + value, 0.0f, Maxhealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return Health/Maxhealth;
}

void UAttributeComponent::HandleStamina(float value)
{
	Stamina = FMath::Clamp(Stamina + value, 0.0f, MaxStamina);
}



float UAttributeComponent::GetStaminaPercent()
{
	return Stamina/MaxStamina;
}

float UAttributeComponent::GetStamina()
{
	return Stamina;
}

float UAttributeComponent::Get_MaxStamina()
{
	return MaxStamina;
}


void UAttributeComponent::RestoreHealth()
{
	Health = Maxhealth;
}

bool UAttributeComponent::IsAlive()
{

	return Health > 0.0f;
}

float UAttributeComponent::Get_Health()
{
	return Health;
}

float UAttributeComponent::Get_MaxHealth()
{
	return Maxhealth;
}

void UAttributeComponent::UseStamaina()
{
	Stamina -= 25.0f;
}



void UAttributeComponent::AddPotion(int32 potion)
{
	PotionAmount += potion;
}

void UAttributeComponent::SubPotion()
{
	PotionAmount -= 1;
}






void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


