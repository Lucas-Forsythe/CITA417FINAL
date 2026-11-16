// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetDummy.generated.h"

class UStaticMeshComponent;
class UHealthComponent;

UCLASS()
class CITA417FINAL_API ATargetDummy : public AActor
{
	GENERATED_BODY()
public:
	ATargetDummy();
protected:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UHealthComponent> Health;
};