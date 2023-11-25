#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 20000.f;
	ProjectileMovementComponent->InitialSpeed = 20000.f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	StartLocation = GetActorLocation();

	
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DistanceDiff = GetActorLocation() - StartLocation;
	if (DistanceDiff.X > PositiveDestroyDistance || DistanceDiff.Y > PositiveDestroyDistance || DistanceDiff.Z > PositiveDestroyDistance)
	{
		DestroyProjectile();
	}

	if (DistanceDiff.X < NegativeDestroyDistance || DistanceDiff.Y < NegativeDestroyDistance || DistanceDiff.Z < PositiveDestroyDistance)
	{
		DestroyProjectile();
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (MyOwner == nullptr) {
		Destroy();
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		if (HitParticles)
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
	}
}

void AProjectileBase::DestroyProjectile()
{
	Destroy();
}
