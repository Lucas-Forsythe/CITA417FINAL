// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CITA417AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CITA417FINAL_API UCITA417AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	

protected:
	UPROPERTY(BlueprintReadOnly, Category="Character State") float Speed = 0.f;
	UPROPERTY(BlueprintReadOnly, Category="Character State") float Direction = 0.f;
	UPROPERTY(BlueprintReadOnly, Category="Character State") bool bIsAirborne = false;
	
	UPROPERTY(BlueprintReadOnly, Category="Character State") bool bIsAttacking = false;
	UPROPERTY(BlueprintReadOnly, Category="Character State") float AttackAlpha = 0.f;

	UPROPERTY(EditAnywhere, Category="Debug") bool bShowDebug = true;

	UPROPERTY() TObjectPtr<class ACITA417FINALCharacter> OwnerCharacter;
};