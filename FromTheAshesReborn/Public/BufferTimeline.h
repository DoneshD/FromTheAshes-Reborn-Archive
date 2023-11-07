// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "BufferTimeline.generated.h"

class UTimelineComponent;
class UCurveFloat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROMTHEASHESREBORN_API UBufferTimeline : public UActorComponent
{
	GENERATED_BODY()


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
	UCurveFloat* BufferCurve;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

	//UPROPERTY(EditAnywhere, Category = "Timeline")
	float ZOffest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* BufferTimeLine;

	UPROPERTY()
	FOnTimelineFloat InterpFunction{};

	UPROPERTY()
	FOnTimelineEvent TimelineFinished{};

	UFUNCTION()
		void TimelineFloatReturn(float value);

	UFUNCTION()
		void OnTimelineFinished();
};
