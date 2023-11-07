// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferTimeline.h"
#include "PlayableCharacter.h"

// Sets default values for this component's properties
UBufferTimeline::UBufferTimeline()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	BufferTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	UpdatedEvent.BindUFunction(this, FName("OnTimelineUpdate"));


}


// Called when the game starts
void UBufferTimeline::BeginPlay()
{
	Super::BeginPlay();

	if (BufferCurve)
	{
		//Add float curve to timeline and connect it to the interpt functions delegates
		BufferTimeLine->AddInterpFloat(BufferCurve, InterpFunction, FName("Alpha"));

		BufferTimeLine->SetTimelineFinishedFunc(UpdatedEvent);

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

void UBufferTimeline::EndPlay()
{
	BufferTimeLine->Stop();
}

void UBufferTimeline::TimelineFloatReturn(float value, FVector CurrentLocation, FVector NewLocation)
{
	BufferTimeLine->PlayFromStart();
	BufferLerp = FMath::Lerp(CurrentLocation, NewLocation, value);

	//APlayableCharacter* PlayableCharacter = Cast<APlayableCharacter>(GetOwner());
	//if (PlayableCharacter)
	//{
		//PlayableCharacter->SetActorLocation(BufferLerp);
	//}

}

void UBufferTimeline::OnTimelineUpdate(FVector NewLocation)
{
	
}

