// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferTimeline.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
//#include "Components/TimelineComponent.h"

// Sets default values for this component's properties
UBufferTimeline::UBufferTimeline()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	BufferTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}


// Called when the game starts
void UBufferTimeline::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBufferTimeline::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

