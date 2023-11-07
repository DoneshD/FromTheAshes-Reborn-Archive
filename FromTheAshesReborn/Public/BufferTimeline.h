// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BufferTimeline.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROMTHEASHESREBORN_API UBufferTimeline : public UActorComponent
{
	GENERATED_BODY()

	class UTimelineComponent* BufferTimeline;

public:	
	// Sets default values for this component's properties
	UBufferTimeline();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* FCurve;

		
};
