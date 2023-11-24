// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class FROMTHEASHESREBORN_API AProjectile : public AActor
{
	GENERATED_BODY()
	

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyProjectile();
private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UParticleSystemComponent* TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UParticleSystem* HitParticles;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Damage = 10;

	float PositiveDestroyDistance = 10000;
	float NegativeDestroyDistance = -10000;
	FVector StartLocation;

	AController* GetOwnerController() const;

public:	

	virtual void Tick(float DeltaTime) override;

	AProjectile();

};
