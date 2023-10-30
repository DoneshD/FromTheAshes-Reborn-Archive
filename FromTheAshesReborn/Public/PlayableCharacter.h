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
	//Constructor
	APlayableCharacter();

	//Casts
	TObjectPtr<AFTACharacter> FTACharacter;


	//Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_LightAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_HeavyAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_LockOn;

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
	void PerformHeavyAttack(int HeavyAttackIndex);

	//Heavy Combos
	void SelectHeavyCombo();
	void NewHeavyCombo();
	void PerformHeavyCombo(TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo);

	UFUNCTION()
	void HeavyAttackPaused();


	// Save Attacks
	UFUNCTION(BlueprintCallable, Category = "Light Attack")
	void SaveLightAttack();

	UFUNCTION(BlueprintCallable, Category = "Heavy Attack")
	void SaveHeavyAttack();

	//Timed Attacks
	void StartAttackPausedTimer();
	void ClearAttackPausedTimer();
	
	//Weapon Collision
	UFUNCTION(BlueprintCallable, Category = "Weapon Collision")
	void StartWeaponCollision();

	UFUNCTION(BlueprintCallable, Category = "Weapon Collision")
	void EndWeaponCollision();

	//Movement
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void EnableRootRotation();

	//LockOn
	void RotationToTarget();
	void SoftLockOn();

	void HardLockOn();

private:

	//Lock Ons
	bool bTargeting = false;

	TObjectPtr<AActor> HardTarget;
	TObjectPtr<AActor> SoftTarget;

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

	//Weapon Collision
	bool bActiveCollision = false;
	TObjectPtr<AActor> HitActor;
	TArray<TObjectPtr<AActor>> AlreadyHitActors;

	//Timers
	FTimerHandle AttackPauseHandle;
	FOnAttackPausedEvent OnAttackPausedEvent;

	//Timelines
	
	//Execution
	bool bExecuting;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> LightAttackCombo;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> HeavyAttackCombo;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo1;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo2;
	

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

};
