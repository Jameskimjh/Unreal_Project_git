// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "Slash_Player.h"
#include "Kismet/GameplayStatics.h"


void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlash_Player* slashCharacter = Cast<ASlash_Player>(OtherActor);

	if (slashCharacter)
	{
		if (PickUpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickUpSound,
				GetActorLocation()
			);
		}
		Destroy();
	}


}
