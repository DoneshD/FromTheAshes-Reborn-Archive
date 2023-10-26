// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

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
}

//-------------------------------------------------- FSM Resets -----------------------------------------------------------------//

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
	SoftTarget = NULL;

	ResetLightAttack();
	ResetHeavyAttack();
	ResetAirAttack();
	ClearAttackPausedTimer();

}

bool APlayableCharacter::CanAttack()
{
	TArray<EStates> MakeArray = { EStates::EState_Attack, EStates::EState_Execution };
	return !GetCharacterMovement()->IsFalling() && !GetCharacterMovement()->IsFlying() && !IsStateEqualToAny(MakeArray);
}

//--------------------------------------------------------- PlayerInputComponent ---------------------------------------------------------------------//

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
		InputComp->BindAction(Input_HeavyAttack, ETriggerEvent::Started, this, &APlayableCharacter::InputHeavyAttack);

	}
}

//------------------------------------------------------------ Movement -----------------------------------------------------------------//

void APlayableCharacter::EnableRootRotation()
{
	if (!SoftTarget && !HardTarget)
	{
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}
}

//------------------------------------------------------------- LockOn -----------------------------------------------------------------//

void APlayableCharacter::SoftLockOn()
{
	if (!bTargeting && !HardTarget)
	{
		FVector EndLocation = (GetCharacterMovement()->GetLastInputVector() * 500.f) + GetActorLocation();
		FHitResult OutHit;

		TArray<AActor*> ActorArray;
		ActorArray.Add(this);
		
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		bool TargetHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
			GetWorld(), 
			GetActorLocation(), 
			EndLocation, 
			100.f, 
			ObjectTypes, 
			false, 
			ActorArray, 
			EDrawDebugTrace::ForDuration, 
			OutHit, 
			true);
		if (TargetHit)
		{
			AActor* HitActor = OutHit.GetActor();
			if (HitActor)
			{
				SoftTarget = HitActor;
			}
		}
		
	}
}

//---------------------------------------------------------- Attack Saves -----------------------------------------------------------------//

void APlayableCharacter::SaveLightAttack()
{
	if (bLightAttackSaved)
	{
		bLightAttackSaved = false;
		TArray<EStates> MakeArray = { EStates::EState_Attack };
		if (IsStateEqualToAny(MakeArray))
		{
			SetState(EStates::EState_Nothing);
		}
		LightAttack();
	}
}

void APlayableCharacter::SaveHeavyAttack()
{
	if (bHeavyAttackSaved)
	{
		bHeavyAttackSaved = false;
		//Air Slam()
		TArray<EStates> MakeArray = { EStates::EState_Attack };
		if (IsStateEqualToAny(MakeArray))
		{
			SetState(EStates::EState_Nothing);
		}
		//Perform pause combo or regular
		if (bHeavyAttackPaused)
		{
			NewHeavyCombo();
		}
		HeavyAttack();
	}
}

void APlayableCharacter::StartAttackPausedTimer()
{
	GetWorldTimerManager().SetTimer(AttackPauseHandle, this, &APlayableCharacter::HeavyAttackPaused, .8, true);
}

void APlayableCharacter::ClearAttackPausedTimer()
{
	GetWorldTimerManager().ClearTimer(AttackPauseHandle);
}

void APlayableCharacter::HeavyAttackPaused()
{
	bHeavyAttackPaused = true;
	OnAttackPausedEvent.Broadcast();
}


//------------------------------------------------------ Light Attack Actions -----------------------------------------------------------------//

void APlayableCharacter::PerformLightAttack(int AttackIndex)
{
	UAnimMontage* CurrentMontage = LightAttackCombo[AttackIndex];
	if (CurrentMontage)
	{
		//StopBuffer()
		//Buffer()
		SetState(EStates::EState_Attack);
		SoftLockOn();
		PlayAnimMontage(CurrentMontage);
		LightAttackIndex++;
		if (LightAttackIndex >= LightAttackCombo.Num())
		{
			LightAttackIndex = 0;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Montage"));
	}

}

void APlayableCharacter::LightAttack()
{
	if (CanAttack())
	{
		//CanLaunch()
		//DodgeAttacks() SHOULD REFACTOR
		ResetHeavyAttack();
		PerformLightAttack(LightAttackIndex);

	}
	else
	{
		//Air attack logic
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

//--------------------------------------------------------- Heavy Attack Actions -----------------------------------------------------------------//


void APlayableCharacter::PerformHeavyCombo(TArray<TObjectPtr<UAnimMontage>> PausedHeavyAttackCombo)
{
	UAnimMontage* AttackMontage = PausedHeavyAttackCombo[NewHeavyAttackIndex];
	if (AttackMontage)
	{
		//StopBuffer();
		//Buffer();
		SetState(EStates::EState_Attack);
		//SoftLock();
		PlayAnimMontage(AttackMontage);
		NewHeavyAttackIndex++;
		if (NewHeavyAttackIndex >= PausedHeavyAttackCombo.Num())
		{
			NewHeavyAttackIndex = 0;
			bHeavyAttackPaused = false;
		}
	}
}

//TODO: Select combo for scalability
void APlayableCharacter::SelectHeavyCombo()
{
	//UE_LOG(LogTemp, Warning, TEXT("HeavyAttackIndex: %d"), NewHeavyAttackIndex);
	if (HeavyAttackIndex == 1)
	{
		PerformHeavyCombo(PausedHeavyAttackCombo1);
	}
	if (HeavyAttackIndex == 2)
	{
		PerformHeavyCombo(PausedHeavyAttackCombo2);

	}


}
void APlayableCharacter::NewHeavyCombo()
{
	TArray<EStates> MakeArray = { EStates::EState_Attack };
	if(!IsStateEqualToAny(MakeArray))
	{
		SelectHeavyCombo();
	}
}

void APlayableCharacter::PerformHeavyAttack(int AttackIndex)
{
	UAnimMontage* CurrentMontage = HeavyAttackCombo[AttackIndex];
	if (CurrentMontage)
	{
		//StopBuffer()
		//Buffer()
		SetState(EStates::EState_Attack);
		//SoftLock()
		PlayAnimMontage(CurrentMontage);
		StartAttackPausedTimer();
		HeavyAttackIndex++;
		if (HeavyAttackIndex >= HeavyAttackCombo.Num())
		{
			HeavyAttackIndex = 0;
			ClearAttackPausedTimer();
			bHeavyAttackPaused = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Montage"));
	}
}

void APlayableCharacter::HeavyAttack()
{
	if (CanAttack())
	{
		//Clear Timer : Attack Paused
		ClearAttackPausedTimer();
		bHeavyAttackPaused = false;
		ResetLightAttack();
		PerformHeavyAttack(HeavyAttackIndex);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error"));
	}
}

void APlayableCharacter::InputHeavyAttack()
{
	bDodgeSaved = false;
	bLightAttackSaved = false;

	//AirSlam()

	TArray<EStates> MakeArray = { EStates::EState_Attack };
	if (IsStateEqualToAny(MakeArray))
	{
		bHeavyAttackSaved = true;
	}
	else
	{
		HeavyAttack();
	}
}
