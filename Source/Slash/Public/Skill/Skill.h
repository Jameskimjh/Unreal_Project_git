#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill.generated.h"

class ASlash_Player;

UCLASS()
class SLASH_API ASkill : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkill();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category = Instigator)
	ASlash_Player* Player;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

private:
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, Category = "Skill Particle")
	UParticleSystemComponent* Particle;

	
};
