#include "BTTask_AttackTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Pawn.h"
#include "WeaponComponent.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
	NodeName = TEXT("Attack Target");
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_AttackTarget, TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AI || !BB) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetKey.SelectedKeyName));
	APawn* Pawn = AI->GetPawn();
	if (!Target || !Pawn) return EBTNodeResult::Failed;

	UWeaponComponent* Weapon = Pawn->FindComponentByClass<UWeaponComponent>();
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackTarget: %s has no WeaponComponent"), *GetNameSafe(Pawn));
		return EBTNodeResult::Failed;
	}

	Weapon->Fire();   // the same component the player uses; cooldown is handled inside it
	return EBTNodeResult::Succeeded;
}