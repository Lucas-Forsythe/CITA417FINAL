#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTService_UpdateCombatState.generated.h"

UCLASS()
class CITA417FINAL_API UBTService_UpdateCombatState : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateCombatState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Combat", meta=(ClampMin="0")) float AttackRange = 800.f;
	UPROPERTY(EditAnywhere, Category="Blackboard") FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category="Blackboard") FBlackboardKeySelector InAttackRangeKey;
};