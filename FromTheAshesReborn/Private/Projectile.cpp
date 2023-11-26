#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/ArrowComponent.h"
#include "PlayableCharacter.h"
#include "MeleeEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 20000.f;
	ProjectileMovementComponent->InitialSpeed = 20000.f;

	

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	StartLocation = GetActorLocation();
	
}


// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DistanceDiff = GetActorLocation() - StartLocation;
	if (DistanceDiff.X > PositiveDestroyDistance || DistanceDiff.Y > PositiveDestroyDistance)
	{
		//Need to remove from the projectile array
		DestroyProjectile();
	}

	if (DistanceDiff.X < NegativeDestroyDistance || DistanceDiff.Y < NegativeDestroyDistance)
	{
		//Need to remove from the projectile array
		DestroyProjectile();
	}

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (MyOwner == nullptr) {
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	AActor* HitActor = Hit.GetActor();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		if (HitParticles)
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());

		UE_LOG(LogTemp, Warning, TEXT("Dude"));

		APlayableCharacter* PlayerCharacter = Cast<APlayableCharacter>(MyOwner);
		PlayerCharacter->bKunaiLanded = true;

		//USE INTERFACES!!!!!!!!!!!
		AMeleeEnemy* Enemy = Cast<AMeleeEnemy>(HitActor);
		if (Enemy)
		{
			PlayerCharacter->bFlank = true;
		}
	}
	
}

void AProjectile::DestroyProjectile()
{
	APlayableCharacter* PlayerCharacter = Cast<APlayableCharacter>(GetOwner());
	Destroy();
	PlayerCharacter->bKunaiLanded = true;
}
