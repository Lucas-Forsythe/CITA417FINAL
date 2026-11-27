#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_FindPatrolPoint.generated.h"

UCLASS()
class CITA417FINAL_API UBTTask_FindPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindPatrolPoint();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category="Patrol", meta=(ClampMin="0")) float PatrolRadius = 800.f;
	UPROPERTY(EditAnywhere, Category="Blackboard") FBlackboardKeySelector HomeKey;
	UPROPERTY(EditAnywhere, Category="Blackboard") FBlackboardKeySelector PatrolLocationKey;
};