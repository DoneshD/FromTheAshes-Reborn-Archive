// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTACharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "PlayableCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackPausedEvent);

/**
 * 
 */

USTRUCT()
struct FSideDodgeArray
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Anim")
	TArray<TObjectPtr<UAnimMontage>> SideDodgeArray;
};

class UTimelineComponent;
class UCurveFloat;

UCLASS()
class FROMTHEASHESREBORN_API APlayableCharacter : public AFTACharacter
{
	GENERATED_BODY()

	
protected:
	//Constructor
	APlayableCharacter();

	//BeginPlay
	virtual void BeginPlay() override;

	//Casts
	TObjectPtr<AFTACharacter> FTACharacter;

	//Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_LightAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_HeavyAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Dodge;

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
	bool CanDodge();

	//Dodge
	void Dodge();
	void PerformDodge();
	void DodgeSystem(float X, float Y);

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void SaveDodge();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void EnableRoll();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void DisableRoll();

	//Light Attacks
	void InputLightAttack();
	void LightAttack();
	void PerformLightAttack(int LightAttackIndex);
	UFUNCTION(BlueprintCallable, Category = "Light Attack")
	void SaveLightAttack();

	//Heavy Attacks
	void InputHeavyAttack();
	void HeavyAttack();
	void PerformHeavyAttack(int HeavyAttackIndex);
	UFUNCTION(BlueprintCallable, Category = "Heavy Attack")
	void SaveHeavyAttack();

	//Heavy Combos
	void SelectHeavyCombo();
	void NewHeavyCombo();
	void PerformHeavyCombo(TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo);

	UFUNCTION()
	void HeavyAttackPaused();

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

	//Timelines
	void StartBuffer(float Amount);
	void StopBuffer();

private:

	//Lock Ons
	bool bTargeting = false;

	TObjectPtr<AActor> HardTarget;
	TObjectPtr<AActor> SoftTarget;

	//Dodge logic
	bool bDodgeSaved;
	bool bCanRoll;
	bool bCanDodge = true;

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
	TObjectPtr<AActor> WCHitActor;
	TArray<TObjectPtr<AActor>> AlreadyHitActors_L;
	TArray<TObjectPtr<AActor>> AlreadyHitActors_R;

	//Timers
	FTimerHandle AttackPauseHandle;
	FOnAttackPausedEvent OnAttackPausedEvent;
	//Execution
	bool bExecuting;

	TMap<int, int> YCardinalMapping;
	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> LightAttackCombo;
	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> HeavyAttackCombo;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo1;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo2;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge Anim")
	TObjectPtr<UAnimMontage> ForwardDodgeAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Anim")
	TObjectPtr<UAnimMontage> BackDodgeAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge Anim")
	TObjectPtr<UAnimMontage> ForwardRollAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Anim")
	TObjectPtr<UAnimMontage> BackRollAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Anim")
	TArray<FSideDodgeArray> CardinalDodgeArray;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Anim")
	TArray<FSideDodgeArray> CardinalRollArray;

	//Timelines
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* BufferCurve;

	UPROPERTY()
	UTimelineComponent* BufferTimeLine;

	UPROPERTY()
	FOnTimelineFloat InterpFunction{};

	UPROPERTY()
	FOnTimelineEvent UpdatedEvent{};

	UFUNCTION()
		void TimelineFloatReturn(float value, FVector CurrentLocation, FVector NewLocation);

	UFUNCTION()
		void OnTimelineUpdate(FVector NewLocation);

	FVector BufferLerp;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

};

