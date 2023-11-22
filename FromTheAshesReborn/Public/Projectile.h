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

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UParticleSystemComponent* TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Damage = 10;

	float PositiveDestroyDistance = 20000;
	float NegativeDestroyDistance = -20000;
	FVector StartLocation;

	AController* GetOwnerController() const;

public:	

	virtual void Tick(float DeltaTime) override;

	AProjectile();

};
