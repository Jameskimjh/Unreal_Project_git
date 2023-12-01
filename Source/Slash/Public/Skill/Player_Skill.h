// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Player_Skill.generated.h"

UCLASS()
class SLASH_API APlayer_Skill : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayer_Skill();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	

};
