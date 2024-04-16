// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshActor.h"

AMeshActor::AMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	DistanceAlongSpline = 0.0f;
}

AMeshActor* AMeshActor::GetMeshActor()
{
	return this;
}

void AMeshActor::SetMesh(UStaticMesh* NewMesh)
{
	Mesh->SetStaticMesh(NewMesh);
}

float AMeshActor::GetDistanceAlongSpline() const
{
	return DistanceAlongSpline;
}

void AMeshActor::SetDistanceAlongSpline(float NewDistance)
{
	DistanceAlongSpline = NewDistance;
}
