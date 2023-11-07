// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferTimeline.h"

// Sets default values for this component's properties
UBufferTimeline::UBufferTimeline()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	BufferTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
}


// Called when the game starts
void UBufferTimeline::BeginPlay()
{
	Super::BeginPlay();

	if (BufferCurve)
	{
		//Add float curve to timeline and connect it to the interpt functions delegates
		BufferTimeLine->AddInterpFloat(BufferCurve, InterpFunction, FName("Alpha"));

		BufferTimeLine->SetTimelineFinishedFunc(TimelineFinished);

		//Game Specific Logic?

		//Set settings
		BufferTimeLine->SetLooping(false);
		BufferTimeLine->SetIgnoreTimeDilation(true);
	}

	// ...
	
}


// Called every frame
void UBufferTimeline::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBufferTimeline::TimelineFloatReturn(float value)
{
	//Lerp logic
}

void UBufferTimeline::OnTimelineFinished()
{
	if (BufferTimeLine->GetPlaybackPosition() == 0.0f)
	{
		GLog->Log("PLAY");
		BufferTimeLine->Play();
	}
	else
	{
		BufferTimeLine->Reverse();
	}
}

