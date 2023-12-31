// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class FROMTHEASHESREBORN_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	void DestroyProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UParticleSystemComponent* TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Damage;

	float PositiveDestroyDistance = 10000;
	float NegativeDestroyDistance = -10000;

	FVector DistanceDiff;
	FVector StartLocation;

public:

	virtual void Tick(float DeltaTime) override;

	AProjectileBase();

};