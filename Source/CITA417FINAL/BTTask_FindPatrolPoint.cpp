#include "BTTask_FindPatrolPoint.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_FindPatrolPoint::UBTTask_FindPatrolPoint()
{
	NodeName = TEXT("Find Patrol Point");
	HomeKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPatrolPoint, HomeKey));
	PatrolLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPatrolPoint, PatrolLocationKey));
}

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AI || !BB) return EBTNodeResult::Failed;

	UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(AI->GetWorld());
	if (!Nav)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindPatrolPoint: no navigation system (is there a NavMeshBoundsVolume?)"));
		return EBTNodeResult::Failed;
	}

	const FVector Home = BB->GetValueAsVector(HomeKey.SelectedKeyName);
	FNavLocation Result;
	if (Nav->GetRandomReachablePointInRadius(Home, PatrolRadius, Result))
	{
		BB->SetValueAsVector(PatrolLocationKey.SelectedKeyName, Result.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}