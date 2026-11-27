#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;

UCLASS()
class CITA417FINAL_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION() void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION() void HandlePawnDeath(AActor* DeadActor);
	void ForgetLastKnownLocation();

	UPROPERTY(EditAnywhere, Category="AI") TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	UPROPERTY(EditAnywhere, Category="AI", meta=(ClampMin="0")) float SearchTimeout = 8.f;

	UPROPERTY(EditAnywhere, Category="AI|Blackboard Keys") FName TargetKey = TEXT("TargetActor");
	UPROPERTY(EditAnywhere, Category="AI|Blackboard Keys") FName LastKnownLocationKey = TEXT("LastKnownLocation");
	UPROPERTY(EditAnywhere, Category="AI|Blackboard Keys") FName HomeLocationKey = TEXT("HomeLocation");

private:
	FTimerHandle SearchTimerHandle;
};