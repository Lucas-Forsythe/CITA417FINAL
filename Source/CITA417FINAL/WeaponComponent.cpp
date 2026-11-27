// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h" 
#include "Kismet/GameplayStatics.h" 
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UWeaponComponent::IsFiring() const
{
	const UWorld* World = GetWorld();
	return World && (World->GetTimeSeconds() - LastFireTime) < AttackAnimDuration;
}

bool UWeaponComponent::Fire()
{
	UWorld* World = GetWorld();
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!World || !OwnerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponComponent: needs a Pawn owner"));
		return false;
	}

	const float Now = World->GetTimeSeconds();
	if (Now - LastFireTime < FireCooldown) return false;     // cooldown

	AController* Controller = OwnerPawn->GetController();
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponComponent: owner has no Controller"));
		return false;
	}
	LastFireTime = Now;

	FVector Start; FRotator ViewRot;
	Controller->GetPlayerViewPoint(Start, ViewRot);
	const FVector Dir = ViewRot.Vector();
	const FVector End = Start + Dir * Range;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(WeaponTrace), false, OwnerPawn);  // ignore self
	FHitResult Hit;
	FCollisionObjectQueryParams ObjParams;
	
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	const bool bHit = World->LineTraceSingleByObjectType(Hit, Start, End, ObjParams, Params);

	if (bDrawDebug)
	{
		DrawDebugLine(World, Start, bHit ? Hit.ImpactPoint : End, bHit ? FColor::Red : FColor::Green, false, 1.f);
		if (bHit) DrawDebugSphere(World, Hit.ImpactPoint, 12.f, 8, FColor::Red, false, 1.f);   // hit indicator
	}

	if (bHit && Hit.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, Dir, Hit, Controller, OwnerPawn,
										   UDamageType::StaticClass());
	}
	return bHit;
}