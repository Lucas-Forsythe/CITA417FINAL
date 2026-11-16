// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class CITA417FINAL_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWeaponComponent();

	UFUNCTION(BlueprintCallable, Category="Weapon") bool Fire();
	UFUNCTION(BlueprintPure, Category="Weapon") bool IsFiring() const;

protected:
	UPROPERTY(EditAnywhere, Category="Weapon") float Damage = 25.f;
	UPROPERTY(EditAnywhere, Category="Weapon") float Range = 5000.f;
	UPROPERTY(EditAnywhere, Category="Weapon") float FireCooldown = 0.4f;       // independent improvement
	UPROPERTY(EditAnywhere, Category="Weapon") float AttackAnimDuration = 0.3f;
	UPROPERTY(EditAnywhere, Category="Weapon") bool bDrawDebug = true;

private:
	float LastFireTime = -1000.f;
};