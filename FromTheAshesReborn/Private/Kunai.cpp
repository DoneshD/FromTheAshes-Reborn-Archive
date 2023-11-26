// Fill out your copyright notice in the Description page of Project Settings.


#include "Kunai.h"


AKunai::AKunai()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AKunai::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Test 3"));

}


void AKunai::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


