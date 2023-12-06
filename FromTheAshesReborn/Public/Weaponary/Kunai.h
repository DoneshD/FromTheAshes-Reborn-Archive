// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "Kunai.generated.h"

/**
 * 
 */
UCLASS()
class FROMTHEASHESREBORN_API AKunai : public AProjectileBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:

public:
	
	virtual void Tick(float DeltaTime) override;

	AKunai();


};
