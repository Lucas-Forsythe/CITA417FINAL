// Fill out your copyright notice in the Description page of Project Settings.

#include "CITA417FINALCharacter.h"
#include "CITA417AnimInstance.h"

void UCITA417AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACITA417FINALCharacter>(TryGetPawnOwner());
}

void UCITA417AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACITA417FINALCharacter>(TryGetPawnOwner());
		if (!OwnerCharacter) return;
	}
	Speed       = OwnerCharacter->GetMovementSpeed();
	Direction   = OwnerCharacter->GetMovementDirection();
	bIsAirborne = OwnerCharacter->IsAirborne();

	if (bShowDebug && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green,
			FString::Printf(TEXT("Speed %.0f | Dir %.0f | Airborne %d"), Speed, Direction, bIsAirborne));
	}
}