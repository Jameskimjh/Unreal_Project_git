
#include "Skill/Skill.h"
#include "HitInterface.h"
#include "Slash/Public/Slash_Player.h"
#include "Enemy.h"

ASkill::ASkill()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(300.0f);
	RootComponent = Sphere;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("particle"));
	Particle->SetupAttachment(Sphere);

	Particle->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	
}

void ASkill::BeginPlay()
{
	Super::BeginPlay();
	
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASkill::OnSphereOverlap);
}

void ASkill::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy)
	{
		
		UGameplayStatics::ApplyDamage(Enemy, 50.0f,this->GetInstigatorController(), this, UDamageType::StaticClass());
		//Instigator�� Actor�� �Ź����µ� �׷� Actor�� �ı����� �� Enemy�� CombatTarget�� nullptr�� �Ǵϱ� 
		// Crash�� ����
		


		
	}


}



void ASkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


