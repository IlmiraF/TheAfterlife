// Fill out your copyright notice in the Description page of Project Settings.


#include "StateResolver.h"
#include "../Source/TheAfterlife/TheAfterlifeTypes.h"

// Sets default values for this component's properties
UStateResolver::UStateResolver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateResolver::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStateResolver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

