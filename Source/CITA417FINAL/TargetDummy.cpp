// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"
#include "UObject/ConstructorHelpers.h"


ATargetDummy::ATargetDummy()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded()) Mesh->SetStaticMesh(Cube.Object);
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}