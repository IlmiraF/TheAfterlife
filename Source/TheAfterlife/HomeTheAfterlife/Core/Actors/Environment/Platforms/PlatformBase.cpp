// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformBase.h"
#include "Components/StaticMeshComponent.h"
#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"

APlatformBase::APlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(TriggerBox);
}