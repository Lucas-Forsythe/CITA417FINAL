#include "BTService_UpdateCombatState.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Pawn.h"

UBTService_UpdateCombatState::UBTService_UpdateCombatState()
{
	NodeName = TEXT("Update Combat State");
	Interval = 0.2f;
	RandomDeviation = 0.f;
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateCombatState, TargetKey), AActor::StaticClass());
	InAttackRangeKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateCombatState, InAttackRangeKey));
}

void UBTService_UpdateCombatState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!BB || !AI || !AI->GetPawn()) return;

	const AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetKey.SelectedKeyName));
	bool bInRange = false;
	if (Target)
	{
		bInRange = FVector::Dist(AI->GetPawn()->GetActorLocation(), Target->GetActorLocation()) <= AttackRange;
	}
	BB->SetValueAsBool(InAttackRangeKey.SelectedKeyName, bInRange);
}