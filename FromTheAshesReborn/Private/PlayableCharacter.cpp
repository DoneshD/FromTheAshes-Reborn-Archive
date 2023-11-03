// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//------------------------------------------------------------- Constructor -----------------------------------------------------------------//

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

//------------------------------------------------------------- FSM Resets -----------------------------------------------------------------//

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
	HardTarget = NULL;
	SoftTarget = NULL;
	bCanDodge = true;
	bDodgeSaved = false;

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

bool APlayableCharacter::CanDodge()
{
	TArray<EStates> MakeArray = { EStates::EState_Dodge, EStates::EState_Execution };
	return !GetCharacterMovement()->IsFalling() && bCanDodge && !IsStateEqualToAny(MakeArray);
}

//------------------------------------------------------------- Tick -----------------------------------------------------------------//

void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bTargeting && HardTarget)
	{
		if (GetDistanceTo(HardTarget) < 2000.f)
		{
			if (GetCharacterMovement()->IsFalling() || GetCharacterMovement()->IsFlying())
			{
				FVector ResultVectorAVector(0, 0, 80.f);
			}

			FVector ResultVector(0, 0, 30.f);
			FVector TargetLocation = HardTarget->GetActorLocation() - ResultVector;
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
			FRotator InterpRot = FMath::RInterpTo(GetControlRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 5.f);

			GetController()->SetControlRotation(InterpRot);
		}
		else
		{
			bTargeting = false;
			HardTarget = NULL;
		}
	}

	//------------------------------------------------------------ TICK::Weapon Collisions -----------------------------------------------------------------//

	if (bActiveCollision)
	{
		TArray<FHitResult> LeftHits;
		FVector StartLocation_L = GetMesh()->GetSocketLocation("Start_L");
		FVector EndLocation_L = GetMesh()->GetSocketLocation("End_L");

		TArray<FHitResult> RightHits;
		FVector StartLocation_R = GetMesh()->GetSocketLocation("Start_R");
		FVector EndLocation_R = GetMesh()->GetSocketLocation("End_R");

		TArray<AActor*> ActorArray;
		ActorArray.Add(this);

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		bool bLeftHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			StartLocation_L,
			EndLocation_L,
			20.f,
			ObjectTypes,
			false,
			ActorArray,
			EDrawDebugTrace::ForDuration,
			LeftHits,
			true);

		bool bRighttHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			StartLocation_R,
			EndLocation_R,
			20.f,
			ObjectTypes,
			false,
			ActorArray,
			EDrawDebugTrace::ForDuration,
			RightHits,
			true);

		//Use INTERFACE
		for (auto& CurrentHit : LeftHits)
		{
			if (CurrentHit.GetActor())
			{
				WCHitActor = CurrentHit.GetActor();
			}
			if (!AlreadyHitActors_L.Contains(CurrentHit.GetActor()))
			{
				AlreadyHitActors_L.AddUnique(WCHitActor);
				//TODO
				//UGameplayStatics::ApplyDamage()
			}
		}

		for (auto& CurrentHit : RightHits)
		{
			if (CurrentHit.GetActor())
			{
				WCHitActor = CurrentHit.GetActor();
			}
			if (!AlreadyHitActors_R.Contains(CurrentHit.GetActor()))
			{
				AlreadyHitActors_R.AddUnique(WCHitActor);
				//TODO
				//UGameplayStatics::ApplyDamage()
			}
		}
	}
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
		InputComp->BindAction(Input_Dodge, ETriggerEvent::Started, this, &APlayableCharacter::Dodge);
		InputComp->BindAction(Input_LockOn, ETriggerEvent::Started, this, &APlayableCharacter::HardLockOn);
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

//------------------------------------------------------------ Dodge -----------------------------------------------------------------//

void APlayableCharacter::EnableRoll()
{
	bCanRoll = true;
}

void APlayableCharacter::DisableRoll()
{
	bCanRoll = false;
}

void APlayableCharacter::SaveDodge()
{
	if (bDodgeSaved)
	{
		bDodgeSaved = false;
		TArray<EStates> MakeArray = { EStates::EState_Dodge };
		//IDK if this is right
		if (IsStateEqualToAny(MakeArray))
		{
			SetState(EStates::EState_Dodge);
		}
		PerformDodge();
	}
}

void APlayableCharacter::PerformDodge()
{
	//StopRotation01()
	//StopRotation02()
	SoftTarget = NULL;
	//StopBuffer()
	//Buffer()
	if (bTargeting)
	{
		FVector2D FloatValue = InputDirection;
		UE_LOG(LogTemp, Warning, TEXT("Float Value X: %f, Y: %f"), FloatValue.X, FloatValue.Y);
		//DodgeSystem();
	}
	else
	{
		PlayAnimMontage(DodgeArray[0]);
	}
	if (bCanRoll)
	{
		PlayAnimMontage(RollArray[0]);
	}
	SetState(EStates::EState_Dodge);
}

void APlayableCharacter::Dodge()
{
	if (CanDodge())
	{
		PerformDodge();
	}
	else
	{
		bDodgeSaved = true;
	}
}

//------------------------------------------------------------- LockOn -----------------------------------------------------------------//

void APlayableCharacter::RotationToTarget()
{
	if (SoftTarget)
	{
		//TODO
	}
}

void APlayableCharacter::SoftLockOn()
{
	if (!bTargeting && !HardTarget)
	{
		FVector EndLocation = (GetCharacterMovement()->GetLastInputVector() * 250) + GetActorLocation();
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
			EDrawDebugTrace::None, 
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
		else
		{
			SoftTarget = NULL;
		}
		
	}
}

void APlayableCharacter::HardLockOn()
{
	if (!bTargeting && !HardTarget)
	{

		if (UCameraComponent* FollowCamera = this->CameraComp)
		{
			FVector CameraVector = FollowCamera->GetForwardVector();
			FVector EndLocation = (CameraVector * 2000.f) + GetActorLocation();
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
					bTargeting = true;
					HardTarget = HitActor;
				}
			}
		}
	}
	else
	{
		bTargeting = false;
		HardTarget = NULL;

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

//------------------------------------------------------------ Weapon Collisions -----------------------------------------------------------------//

void APlayableCharacter::StartWeaponCollision()
{
	AlreadyHitActors_L.Empty();
	AlreadyHitActors_R.Empty();
	bActiveCollision = true;
}
void APlayableCharacter::EndWeaponCollision()
{
	bActiveCollision = false;

}