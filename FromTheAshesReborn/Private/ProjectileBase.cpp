#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectileInterface.h"
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
	if (DistanceDiff.X > PositiveDestroyDistance || DistanceDiff.Y > PositiveDestroyDistance)
	{
		DestroyProjectile();
	}

	if (DistanceDiff.X < NegativeDestroyDistance || DistanceDiff.Y < NegativeDestroyDistance)
	{
		DestroyProjectile();
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetOwner() == nullptr) {
		DestroyProjectile();
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{

		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		}

		IProjectileInterface* PlayerInterface = Cast<IProjectileInterface>(GetOwner());
		IProjectileInterface* EnemyInterface = Cast<IProjectileInterface>(Hit.GetActor());

		if (PlayerInterface)
		{
			PlayerInterface->SetKunaiLanded();
		}

		if (EnemyInterface)
		{
			PlayerInterface->SetFlank();
		}

	}
}

void AProjectileBase::DestroyProjectile()
{
	IProjectileInterface* PlayerInterface = Cast<IProjectileInterface>(GetOwner());
	PlayerInterface->SetKunaiLanded();
	Destroy();
}
