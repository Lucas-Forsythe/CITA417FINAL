// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class CITA417FINAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UHealthComponent();

	UPROPERTY(BlueprintAssignable) FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable) FOnDeath OnDeath;

	UFUNCTION(BlueprintPure) float GetHealth() const { return CurrentHealth; }
	UFUNCTION(BlueprintPure) bool IsDead() const { return bIsDead; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
							 AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, Category="Health", meta=(ClampMin="1")) float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, Category="Health") bool bDestroyOwnerOnDeath = true;
	UPROPERTY(EditAnywhere, Category="Health") float DestroyDelay = 2.f;
	UPROPERTY(EditAnywhere, Category="Health") bool bShowDebugMessages = true;

private:
	float CurrentHealth = 0.f;
	bool bIsDead = false;
};