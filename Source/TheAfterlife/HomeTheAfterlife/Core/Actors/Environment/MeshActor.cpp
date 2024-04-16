// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshActor.h"

AMeshActor::AMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

AMeshActor* AMeshActor::GetMeshActor()
{
	return this;
}

void AMeshActor::SetMesh(UStaticMesh* NewMesh)
{
	Mesh->SetStaticMesh(NewMesh);
}