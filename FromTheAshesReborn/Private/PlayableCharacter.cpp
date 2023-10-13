// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APlayableCharacter::APlayableCharacter()
{
	//Jump and Air Control
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.7f;
	GetCharacterMovement()->AirControlBoostMultiplier = 6.f;

	//Walk and crouch
	GetCharacterMovement()->SetCrouchedHalfHeight(48.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	ResetState();
}

void APlayableCharacter::ResetLightAttack()
{
	LightAttackIndex = 0;
	bLightAttackSaved = false;
}

void APlayableCharacter::ResetHeavyAttack()
{
	HeavyAttackIndex = 0;
	NewHeavyAttackIndex = 0;

	bHeavyAttackSaved = false;
	bHeavyAttackPaused = false;

	//TODO: Clear Timer by Function Name
}

void APlayableCharacter::ResetAirAttack()
{
	AirComboIndex = 0;
	bLaunched = false;
}

void APlayableCharacter::ResetDodge()
{
	bDodgeSaved = false;
	bCanRoll = false;
	bCanDodge = true;
}

void APlayableCharacter::ResetState()
{
	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}

	SetState(EStates::EState_Nothing);

	ResetLightAttack();
	ResetHeavyAttack();
	ResetAirAttack();
}

bool APlayableCharacter::CanAttack()
{
	TArray<EStates> MakeArray = { EStates::EState_Attack, EStates::EState_Execution };
	return !GetCharacterMovement()->IsFalling() && !GetCharacterMovement()->IsFlying() && !IsStateEqualToAny(MakeArray);
}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			check(Subsystem);

			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultInputMapping, 0);
		}
	}

	UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (InputComp)
	{
		InputComp->BindAction(Input_LightAttack, ETriggerEvent::Started, this, &APlayableCharacter::InputLightAttack);
	}
}

void APlayableCharacter::PerformLightAttack(int AttackIndex)
{

}

void APlayableCharacter::LightAttack()
{
	if (CanAttack())
	{
		//CanLaunch()
		//DodgeAttacks SHOULD REFACTOR
		ResetHeavyAttack();
		PerformLightAttack(LightAttackIndex);

	}
	else 
	{
		//Air attack logc
	}
}

void APlayableCharacter::InputLightAttack()
{
	//Light attack held to true
	bDodgeSaved = false;
	//condition for assassinate
	bHeavyAttackSaved = false;

	TArray<EStates> MakeArray = { EStates::EState_Attack };
	if (IsStateEqualToAny(MakeArray))
	{
		bLightAttackSaved = true;
	}
	else 
	{
		LightAttack();
	}
}
