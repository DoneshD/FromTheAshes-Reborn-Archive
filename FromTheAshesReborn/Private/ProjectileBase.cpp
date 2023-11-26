#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlayableCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "MeleeEnemy.h"
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
	UE_LOG(LogTemp, Warning, TEXT("Construct"));

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

	AActor* MyOwner = GetOwner();

	if (MyOwner == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Test 1"));
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test 2"));

		if (HitParticles)
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());


		IProjectileInterface* Interface = Cast<IProjectileInterface>(MyOwner);
		if (Interface)
		{
			Interface->SetKunaiLanded();
			Interface->Print();
		}

	}
}

void AProjectileBase::DestroyProjectile()
{
	APlayableCharacter* PlayerCharacter = Cast<APlayableCharacter>(GetOwner());
	Destroy();
	PlayerCharacter->bKunaiLanded = true;
}
