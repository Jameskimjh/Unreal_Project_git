// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API AHealthPotion : public AItem
{
	GENERATED_BODY()
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


private:
	UPROPERTY(EditAnywhere,Category = "Potion Properties")
	int32 HealthPotion;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* PickUpSound;
public:
	int32 GetPotion() const { return HealthPotion; }
};
