#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles"));
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
		UE_LOG(LogTemp, Warning, TEXT("Test"));
		Destroy();
	}


	if (DistanceDiff.X < NegativeDestroyDistance || DistanceDiff.Y < NegativeDestroyDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test"));
		Destroy();
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
			UE_LOG(LogTemp, Warning, TEXT("Particles"));
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
		APlayableCharacter* PlayerCharacter = Cast<APlayableCharacter>(MyOwner);
		PlayerCharacter->bKunaiLanded = true;
	}
	
}
