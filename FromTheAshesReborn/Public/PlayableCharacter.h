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
	//-----------------------------------------Constructor------------------------------------------
	APlayableCharacter();

	//-----------------------------------------BeginPlay--------------------------------------------
	virtual void BeginPlay() override;

	//-----------------------------------------Inputs-----------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_LightAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_HeavyAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Dodge;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_LockOn;

	//-----------------------------------------FSM Reset States-------------------------------------
	UFUNCTION(BlueprintCallable, Category = "FSM")
	void ResetState();

	void ResetLightAttack();
	void ResetHeavyAttack();
	void ResetAirAttack();
	void ResetDodge();
	void ResetCombos();

	//-----------------------------------------Movement---------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void EnableRootRotation();

	//-----------------------------------------FSM Attack Check-------------------------------------
	bool CanAttack();
	bool CanDodge();


	//-----------------------------------------Light Attacks----------------------------------------
	void InputLightAttack();
	void LightAttack();
	void PerformLightAttack(int LightAttackIndex);

	UFUNCTION(BlueprintCallable, Category = "Light Attack")
	void SaveLightAttack();

	//-----------------------------------------Heavy Attacks----------------------------------------
	void InputHeavyAttack();
	void HeavyAttack();
	void PerformHeavyAttack(int HeavyAttackIndex);

	UFUNCTION(BlueprintCallable, Category = "Heavy Attack")
	void SaveHeavyAttack();

	//-----------------------------------------Pause Combos-----------------------------------------
	void SelectHeavyPauseCombo();
	void NewHeavyCombo();
	void PerformHeavyPauseCombo(TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo);

	UFUNCTION()
	void HeavyAttackPaused();

	void StartAttackPausedTimer();
	void ClearAttackPausedTimer();

	//-----------------------------------------Attack Strings---------------------------------------
	void PerformComboExtender(int ComboExtenderIndex);
	void PerformComboFinisher();
	void PerformComboSurge();

	//-----------------------------------------Weapon Collision-------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Weapon Collision")
	void StartWeaponCollision();

	UFUNCTION(BlueprintCallable, Category = "Weapon Collision")
	void EndWeaponCollision();

	bool WeaponTrace(TArray<FHitResult>& Hit, FVector& StartLocation, FVector& EndLocation);


	//-----------------------------------------Dodge------------------------------------------------
	void Dodge();
	void PerformDodge();
	void DodgeSystem(float X, float Y);

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void SaveDodge();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void EnableRoll();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void DisableRoll();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void EnableDodge();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void DisableDodge();
	//-----------------------------------------LockOn----------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Lock On")
	void RotationToTarget();

	void StopRotation();

	void SoftLockOn(float Distance);
	void HardLockOn();

	//-----------------------------------------Timelines--------------------------------------------
	void StartBuffer();
	void StopBuffer();

	UFUNCTION()
	void TimelineFloatReturn(float value);

	UFUNCTION()
	void OnTimelineFinished();

private:
	//-----------------------------------------Light Attack-----------------------------------------
	int LightAttackIndex = 0;
	bool bLightAttackSaved;

	//-----------------------------------------Heavy Attack-----------------------------------------
	int HeavyAttackIndex = 0;
	int NewHeavyAttackIndex = 0;
	bool bHeavyAttackSaved;
	bool bHeavyAttackPaused;

	//-----------------------------------------Combo Strings---------------------------------------
	int ComboExtenderIndex = 0;
	int ComboSurgeCount = 0;
	float ComboSurgeSpeed = 1.0;

	bool BranchFinisher = false;

	//-----------------------------------------Air attack-------------------------------------------
	int AirComboIndex;
	bool bLaunched;

	//-----------------------------------------Weapon Collision-------------------------------------
	bool bActiveCollision = false;
	TObjectPtr<AActor> WCHitActor;
	TArray<TObjectPtr<AActor>> AlreadyHitActors_L;
	TArray<TObjectPtr<AActor>> AlreadyHitActors_R;

	//-----------------------------------------Dodge-----------------------------------------------
	bool bDodgeSaved;
	bool bCanRoll;
	bool bCanDodge = true;
	TMap<int, int> YCardinalMapping;

	//-----------------------------------------Lock Ons--------------------------------------------
	bool bTargeting = false;
	FVector TargetRotateLocation;

	TObjectPtr<AActor> HardTarget;
	TObjectPtr<AActor> SoftTarget;

	//-----------------------------------------Timers----------------------------------------------
	FTimerHandle AttackPauseHandle;
	FOnAttackPausedEvent OnAttackPausedEvent;

	//-----------------------------------------Timelines-------------------------------------------
	UTimelineComponent* Timeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* BufferCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* RotationCurve;

	FOnTimelineFloat InterpFunction{};
	FOnTimelineEvent TimelineFinished{};

	float BufferAmount;

	//-----------------------------------------Execution-------------------------------------------
	bool bExecuting;

	//-----------------------------------------Anim Montages---------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> LightAttackCombo;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> HeavyAttackCombo;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> ComboExtender;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> ComboExtenderFinishers;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo1;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Anim")
	TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo2;

	UPROPERTY(EditDefaultsOnly, Category = "Surge")
	TObjectPtr<UAnimMontage> ComboSurge_L;

	UPROPERTY(EditDefaultsOnly, Category = "Bybass")
	TObjectPtr<UAnimMontage> ComboBybass;

	UPROPERTY(EditDefaultsOnly, Category = "Surge")
	TObjectPtr<UAnimMontage> ComboSurge_R;

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

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

};

