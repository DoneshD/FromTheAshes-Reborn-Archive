// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EStates.h"
#include "InputAction.h"
#include "FTACharacter.generated.h"

class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;


UCLASS()
class FROMTHEASHESREBORN_API AFTACharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_LookMouse;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_LookStick;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Jump;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	//Basic controls
	void Move(const FInputActionInstance& Instance);
	void LookMouse(const FInputActionValue& InputValue);
	void LookStick(const FInputActionValue& InputValue);

	//FSM checks
	bool CanJump();
	bool CanDodge();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool bCanDodge;

private:
	UFUNCTION(BlueprintPure, Category = "FSM")
	EStates GetState() const;

	UFUNCTION(Category = "FSM")
	void SetState(EStates NewState);

	UFUNCTION(BlueprintPure, Category = "FSM")
	bool IsStateEqualToAny(TArray<EStates> StatesToCheck);

public:
	AFTACharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	EStates CurrentState;
};
