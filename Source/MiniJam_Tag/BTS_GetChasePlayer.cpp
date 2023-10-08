// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GetChasePlayer.h"
#include "EngineUtils.h"
#include "TagPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_GetChasePlayer::UBTS_GetChasePlayer()
{
	NodeName = TEXT("Determine Chase Player");
}

void UBTS_GetChasePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//TArray<ATagPlayerCharacter*> Players;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) { return; }

	ATagPlayerCharacter* AICharacter = Cast<ATagPlayerCharacter>(AIController->GetPawn());
	if (!AICharacter) { return; }
	//bool AmIIt = GameMode->GetItPlayer() == AICharacter;

	float minDistance = INFINITY;
	ATagPlayerCharacter* ClosestCharacter = 0;
	for (ATagPlayerCharacter* Character : TActorRange<ATagPlayerCharacter>(GetWorld()))
	{
		// Don't Calc your own distance
		if (Character != AICharacter)
		{
			float DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), Character->GetActorLocation());
			if (DistanceToPlayer < minDistance)
			{
				ClosestCharacter = Character;
				minDistance = DistanceToPlayer;
			}
		}
	}

	if (ClosestCharacter)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), ClosestCharacter);
	}
}
