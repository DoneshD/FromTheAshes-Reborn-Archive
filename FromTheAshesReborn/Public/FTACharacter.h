// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EStates.h"
#include "ProjectileInterface.h"
#include "InputAction.h"
#include "FTACharacter.generated.h"

class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;

UCLASS()
class FROMTHEASHESREBORN_API AFTACharacter : public ACharacter, public IProjectileInterface
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UArrowComponent> FrontArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UArrowComponent> BackArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UArrowComponent> LeftArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UArrowComponent> RightArrow;

	//Basic controls
	void Move(const FInputActionInstance& Instance);
	void LookMouse(const FInputActionValue& InputValue);
	void LookStick(const FInputActionValue& InputValue);
	

	void DoubleJump();
	void StopJump();

	//FSM checks
	bool CanJump();
	bool bCanDodge;

	FVector2D InputDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	int JumpCount = 0;

	//FSM functions
	EStates GetState() const;
	void SetState(EStates NewState);
	bool IsStateEqualToAny(TArray<EStates> StatesToCheck);

public:
	AFTACharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void SetKunaiLanded() override;

	UFUNCTION()
	virtual void SetFlank() override;

	EStates CurrentState;

	bool bKunaiLanded = true;

	bool bFlank = false;

	
};
