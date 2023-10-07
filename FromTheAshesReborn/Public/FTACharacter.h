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
	// Called when the game starts or when spawned
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

	void Move(const FInputActionInstance& Instance);

	void LookMouse(const FInputActionValue& InputValue);

	void LookStick(const FInputActionValue& InputValue);

private:
	UFUNCTION(BlueprintPure, Category = "FSM")
	EStates GetState() const;

	UFUNCTION(Category = "FSM")
	void SetState(EStates NewState);

	UFUNCTION(BlueprintPure, Category = "FSM")
	bool IsStateEqualToAny(TArray<EStates> StatesToCheck);

public:
	AFTACharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	EStates CurrentState;

};
