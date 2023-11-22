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
	AProjectile();

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TObjectPtr<UParticleSystemComponent> TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TObjectPtr<UParticleSystemComponent> HitParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Damage = 10;

	float PositiveDestroyDistance = 20000;
	float NegativeDestroyDistance = -20000;
	FVector StartLocation;

	AController* GetOwnerController() const;

public:	

	virtual void Tick(float DeltaTime) override;

};
