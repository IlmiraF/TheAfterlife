// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkourLedge.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AParkourLedge::AParkourLedge() 
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LedgeRoot"));

	LedgeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ledge"));
	LedgeMeshComponent->SetupAttachment(RootComponent);

	LedgeInteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Volume"));
	LedgeInteractionVolume->SetupAttachment(RootComponent);
}

FVector AParkourLedge::GetUpPointOfLocation() const
{
	FVector Point = GetActorLocation() + UpPointOfLocation;
	return Point;
}

FVector AParkourLedge::GetDownPointOfLocation() const
{
	FVector Point = GetActorLocation() + DownPointOfLocation;
	return Point;
}