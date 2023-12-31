

#include "BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Treasure.h"
#include "HealthPotion.h"
#include "Components/CapsuleComponent.h"


ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);

	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule -> SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("BreakableActor"));
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken) return;

	bBroken = true;

	UWorld* World = GetWorld();

	//if (World && Treasureclasses.Num() >0 )
	//{
	//	FVector Location = GetActorLocation();
	//	Location.Z += 75.0f;

	//	const int32 Selection = FMath::RandRange(0, Treasureclasses.Num() - 1);


	//	World->SpawnActor<ATreasure>(Treasureclasses[Selection], Location, GetActorRotation());

	//}
	if (World && HealthPostion)
	{
		FVector Location = GetActorLocation();
		World->SpawnActor<AHealthPotion>(HealthPostion, Location, GetActorRotation());
	}
	
}

void ABreakableActor::GetSkill_Implementation()
{
	if (bBroken) return;

	bBroken = true;
	
	UE_LOG(LogTemp, Warning, TEXT("GetSkill_Breakable"));;

	UWorld* World = GetWorld();

	if (World && HealthPostion)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetSkill_Breakable23"));;

		FVector Location = GetActorLocation();
		World->SpawnActor<AHealthPotion>(HealthPostion, Location, GetActorRotation());
	}
}

