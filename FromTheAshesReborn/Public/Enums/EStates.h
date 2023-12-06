// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */


UENUM(BlueprintType) enum class EStates : uint8
{

	EState_Nothing			UMETA(DisplayName = "Nothing"),
	EState_Attack			UMETA(DisplayName = "Attack"),
	EState_Dodge		UMETA(DisplayName = "Dodge"),
	EState_Execution	UMETA(DisplayName = "Execution")

};
