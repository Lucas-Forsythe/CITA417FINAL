#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AEnemyAIController::AEnemyAIController()
{
	UAIPerceptionComponent* Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*Perception);

	UAISenseConfig_Sight* Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	Sight->SightRadius = 1500.f;
	Sight->LoseSightRadius = 1800.f;
	Sight->PeripheralVisionAngleDegrees = 70.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(Sight->GetSenseImplementation());
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	if (UAIPerceptionComponent* Perception = GetAIPerceptionComponent())
	{
		Perception->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AEnemyAIController::HandleTargetPerceptionUpdated);
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!BehaviorTreeAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: BehaviorTreeAsset is not assigned"), *GetName());
		return;
	}
	if (RunBehaviorTree(BehaviorTreeAsset))
	{
		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			BB->SetValueAsVector(HomeLocationKey, InPawn->GetActorLocation());
		}
	}

	if (UHealthComponent* Health = InPawn->FindComponentByClass<UHealthComponent>())
	{
		Health->OnDeath.AddUniqueDynamic(this, &AEnemyAIController::HandlePawnDeath);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: pawn has no HealthComponent"), *GetName());
	}
}

void AEnemyAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Only react to the player, not other AI
	const APawn* SensedPawn = Cast<APawn>(Actor);
	if (!SensedPawn || !SensedPawn->IsPlayerControlled()) return;

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		GetWorldTimerManager().ClearTimer(SearchTimerHandle);
		BB->SetValueAsObject(TargetKey, Actor);
		BB->ClearValue(LastKnownLocationKey);
		SetFocus(Actor);
	}
	else
	{
		FVector LastKnown = Stimulus.StimulusLocation;
		if (LastKnown.IsNearlyZero()) LastKnown = Actor->GetActorLocation();

		BB->ClearValue(TargetKey);
		BB->SetValueAsVector(LastKnownLocationKey, LastKnown);
		ClearFocus(EAIFocusPriority::Gameplay);
		GetWorldTimerManager().SetTimer(SearchTimerHandle, this,
			&AEnemyAIController::ForgetLastKnownLocation, SearchTimeout, false);
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
			FString::Printf(TEXT("%s: target %s"), *GetNameSafe(GetPawn()),
				Stimulus.WasSuccessfullySensed() ? TEXT("SEEN") : TEXT("LOST")));
	}
}

void AEnemyAIController::ForgetLastKnownLocation()
{
	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->ClearValue(LastKnownLocationKey);   // search over, so the tree falls back to patrol
	}
}

void AEnemyAIController::HandlePawnDeath(AActor* DeadActor)
{
	StopMovement();
	ClearFocus(EAIFocusPriority::Gameplay);
	if (BrainComponent) BrainComponent->StopLogic(TEXT("Pawn died"));
	if (ACharacter* Char = Cast<ACharacter>(GetPawn()))
	{
		Char->GetCharacterMovement()->DisableMovement();
	}
}