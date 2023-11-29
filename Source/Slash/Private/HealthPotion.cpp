// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"
#include "Slash/Public/PickUpInterface.h"
#include "Slash/Public/SlashOverlay.h"
#include "Slash/Public/AttributeComponent.h"

void AHealthPotion::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	
	if (PickUpInterface)
	{
		PickUpInterface->AddPotion(this); // Slash_Player�� ĳ���� �Ǵµ� �ű⼭ AddPotion �Լ��� ȣ���Ѵ�.

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
