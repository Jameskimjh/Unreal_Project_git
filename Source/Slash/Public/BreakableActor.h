// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "BreakableActor.generated.h"


class UGeometryCollectionComponent;
UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint , AActor* Hitter) override;
	virtual void GetSkill_Implementation() override;

protected:
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> Treasureclasses;

	UPROPERTY(EditAnywhere, Category = "HealthPotion")
	TSubclassOf<class AHealthPotion> HealthPostion;

	bool bBroken = false;


};
