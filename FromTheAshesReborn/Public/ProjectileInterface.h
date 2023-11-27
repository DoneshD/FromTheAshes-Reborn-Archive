// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProjectileInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class FROMTHEASHESREBORN_API IProjectileInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void SetFlank() = 0;

	UFUNCTION()
	virtual void SetKunaiLanded() = 0;
};
