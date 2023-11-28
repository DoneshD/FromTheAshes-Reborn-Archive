// Fill out your copyright notice in the Description page of Project Settings.
#include "FTACharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

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

	FrontArrow = CreateDefaultSubobject<UArrowComponent>("FrontArrowComp");
	FrontArrow->SetupAttachment(RootComponent);

	BackArrow = CreateDefaultSubobject<UArrowComponent>("BackArrowComp");
	BackArrow->SetupAttachment(RootComponent);

	LeftArrow = CreateDefaultSubobject<UArrowComponent>("LeftArrowComp");
	LeftArrow->SetupAttachment(RootComponent);

	RightArrow = CreateDefaultSubobject<UArrowComponent>("RightArrowComp");
	RightArrow->SetupAttachment(RootComponent);

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

//--------------------------------------------------------- FSM -----------------------------------------------------------------//

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
	return StatesToCheck.Contains(CurrentState);
}

bool AFTACharacter::CanJump()
{
	TArray<EStates> MakeArray = { EStates::EState_Attack, EStates::EState_Dodge };
	return !IsStateEqualToAny(MakeArray);
}

//--------------------------------------------------------- Enchanced Input -----------------------------------------------------------------//


void AFTACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &AFTACharacter::Move);
		InputComp->BindAction(Input_Jump, ETriggerEvent::Started, this, &AFTACharacter::DoubleJump);
		InputComp->BindAction(Input_Jump, ETriggerEvent::Completed, this, &AFTACharacter::StopJump);

		InputComp->BindAction(Input_LookMouse, ETriggerEvent::Triggered, this, &AFTACharacter::LookMouse);
		InputComp->BindAction(Input_LookStick, ETriggerEvent::Triggered, this, &AFTACharacter::LookStick);
	}
}
//--------------------------------------------------------- Interfaces -----------------------------------------------------------------//

void AFTACharacter::SetKunaiLanded()
{
	bKunaiLanded = true;
}

void AFTACharacter::SetFlank()
{
	bFlank = true;
}

//--------------------------------------------------------- Movement -----------------------------------------------------------------//

void AFTACharacter::DoubleJump()
{
	if (CanJump())
	{
		Jump();
		JumpCount++;
		if (!GetCharacterMovement()->IsFalling())
		{
			JumpCount = 0;
		}
		else
		{
			if (JumpCount < 2)
			{
				Jump();
			}
		}
	}
}

void AFTACharacter::StopJump()
{
	StopJumping();
	JumpCount = 0;
}

void AFTACharacter::Move(const FInputActionInstance& Instance)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();
	InputDirection = AxisValue;

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