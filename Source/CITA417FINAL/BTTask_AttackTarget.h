#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_AttackTarget.generated.h"

UCLASS()
class CITA417FINAL_API UBTTask_AttackTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AttackTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category="Blackboard") FBlackboardKeySelector TargetKey;
};