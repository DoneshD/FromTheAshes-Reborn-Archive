// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTACharacter.h"
#include "Interfaces/EnemyInterface.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class FROMTHEASHESREBORN_API AEnemyBase : public AFTACharacter, public IEnemyInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:


public:
	AEnemyBase();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual AEnemyBase* ReturnSelf() override;

	
};
