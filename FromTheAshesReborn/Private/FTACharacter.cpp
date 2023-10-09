// Fill out your copyright notice in the Description page of Project Settings.
#include "FTACharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FTACharacter)

// Sets default values
AFTACharacter::AFTACharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 1700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	RootComponent = GetCapsuleComponent();

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	SpringArmComp->SetUsingAbsoluteRotation(true);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
}

void AFTACharacter::BeginPlay()
{
	Super::BeginPlay();
	bCanDodge = true;
}

void AFTACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EStates AFTACharacter::GetState() const
{
	return CurrentState;
}

void AFTACharacter::SetState(EStates NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
	}
}

bool AFTACharacter::IsStateEqualToAny(TArray<EStates> StatesToCheck)
{
	if (StatesToCheck.Contains(CurrentState))
	{
		return true;
	}
	return false;
}

bool AFTACharacter::CanJump()
{
	TArray<EStates> MakeArray = { EStates::EState_Execution, EStates::EState_Dodge };
	bool inState = IsStateEqualToAny(MakeArray);
	return !inState && bCanDodge && !GetCharacterMovement()->IsFalling();
}

bool AFTACharacter::CanDodge()
{
	TArray<EStates> MakeArray = { EStates::EState_Attack, EStates::EState_Dodge };
	return !IsStateEqualToAny(MakeArray);
}

void AFTACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	Subsystem->AddMappingContext(DefaultInputMapping, 0);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &AFTACharacter::Move);
	InputComp->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);

	InputComp->BindAction(Input_LookMouse, ETriggerEvent::Triggered, this, &AFTACharacter::LookMouse);
	InputComp->BindAction(Input_LookStick, ETriggerEvent::Triggered, this, &AFTACharacter::LookStick);

}

void AFTACharacter::Move(const FInputActionInstance& Instance)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

	// Move forward/back
	AddMovementInput(ControlRot.Vector(), AxisValue.Y);

	// Move Right/Left strafe
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, AxisValue.X);
}

void AFTACharacter::LookMouse(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();

	AddControllerYawInput(Value.X);
	AddControllerPitchInput(-Value.Y);
}

void AFTACharacter::LookStick(const FInputActionValue& InputValue)
{
	FVector2D Value = InputValue.Get<FVector2D>();

	bool XNegative = Value.X < 0.f;
	bool YNegative = Value.Y < 0.f;

	static const float LookYawRate = 100.0f;
	static const float LookPitchRate = 50.0f;

	Value = Value * Value;

	if (XNegative)
	{
		Value.X *= -1.f;
	}
	if (YNegative)
	{
		Value.Y *= -1.f;
	}

	AddControllerYawInput(Value.X * (LookYawRate) * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(Value.Y * (LookPitchRate) * GetWorld()->GetDeltaSeconds());
}