// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Player_Skill.h"

// Sets default values
APlayer_Skill::APlayer_Skill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	BoxComponent->SetBoxExtent(FVector(1.25f, 1.25f, 1.25f), true);


}

// Called when the game starts or when spawned
void APlayer_Skill::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(APlayer_Skill::)
}

// Called every frame
void APlayer_Skill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

