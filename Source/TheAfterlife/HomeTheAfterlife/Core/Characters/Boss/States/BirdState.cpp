// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdState.h"

UBirdState::UBirdState()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBirdState::Enter()
{
	Super::Enter();

}

void UBirdState::Exit()
{
	Super::Exit();

	// ...
	
}


void UBirdState::BeginPlay()
{
	Super::BeginPlay();
	
}


void UBirdState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

