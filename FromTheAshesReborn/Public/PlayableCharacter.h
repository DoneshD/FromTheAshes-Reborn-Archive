// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTACharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FROMTHEASHESREBORN_API APlayableCharacter : public AFTACharacter
{
	GENERATED_BODY()

	
protected:
	APlayableCharacter();
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_LightAttack;

	//FSM Reset States
	void ResetState();
	void ResetLightAttack();
	void ResetHeavyAttack();
	void ResetAirAttack();
	void ResetDodge();

	//Light attacks
	void LightAttack();

private:

	//Dodge logic
	bool bDodgeSaved;
	bool bCanRoll;
	bool bCanDodge;

	//Light Attack
	int LightAttackIndex;
	bool bLightAttackSaved;

	//Heavy Attack
	int HeavyAttackIndex;
	int NewHeavyAttackIndex;
	bool bHeavyAttackSaved;
	bool bHeavyAttackPaused;

	//Air attack
	int AirComboIndex;
	bool bLaunched;


	bool bExecuting;

public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
