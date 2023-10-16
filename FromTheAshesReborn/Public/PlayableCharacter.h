// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTACharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackPausedEvent);

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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_HeavyAttack;

	//FSM Reset States
	UFUNCTION(BlueprintCallable, Category = "FSM")
	void ResetState();

	void ResetLightAttack();
	void ResetHeavyAttack();
	void ResetAirAttack();
	void ResetDodge();

	//FSM Attack Check
	bool CanAttack();

	//Light Attacks
	void InputLightAttack();
	void LightAttack();
	void PerformLightAttack(int LightAttackIndex);

	//Heavy Attacks
	void InputHeavyAttack();
	void HeavyAttack();
	void NewHeavyCombo();
	void PerformHeavyAttack(int HeavyAttackIndex);
	void SelectHeavyCombo();
	void PerformHeavyCombo();


	// Save Attacks
	UFUNCTION(BlueprintCallable, Category = "Light Attack")
	void SaveLightAttack();

	UFUNCTION(BlueprintCallable, Category = "Light Attack")
	void SaveHeavyAttack();

	//Timed Attacks
	void StartAttackPausedTimer();
	void ClearAttackPausedTimer();

	UFUNCTION()
	void HeavyAttackPaused();



private:

	//Dodge logic
	bool bDodgeSaved;
	bool bCanRoll;
	bool bCanDodge;

	//Light Attack
	int LightAttackIndex = 0;
	bool bLightAttackSaved;

	//Heavy Attack
	int HeavyAttackIndex = 0;
	int NewHeavyAttackIndex = 0;
	bool bHeavyAttackSaved;
	bool bHeavyAttackPaused;

	//Air attack
	int AirComboIndex;
	bool bLaunched;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> LightAttackCombo;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> HeavyAttackCombo;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo;

	//Timers
	FTimerHandle AttackPauseHandle;
	FOnAttackPausedEvent OnAttackPausedEvent;

	bool bExecuting;

	

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
