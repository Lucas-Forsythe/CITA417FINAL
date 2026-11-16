// Fill out your copyright notice in the Description page of Project Settings.
#include "HealthComponent.h"
#include "Engine/Engine.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthComponent has no owner"));
		return;
	}
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
										   AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead || Damage <= 0.f) return;          // death can only happen once

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (bShowDebugMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red,
			FString::Printf(TEXT("%s HP: %.0f / %.0f"), *GetNameSafe(DamagedActor), CurrentHealth, MaxHealth));
	}

	if (CurrentHealth <= 0.f)
	{
		bIsDead = true;
		OnDeath.Broadcast(DamagedActor);
		if (bDestroyOwnerOnDeath && DamagedActor)
		{
			DamagedActor->SetActorEnableCollision(false);   // disabled state
			DamagedActor->SetLifeSpan(DestroyDelay);
		}
	}
}