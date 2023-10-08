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

	TArray<ATagPlayerCharacter*> Players;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) { return; }

	ATagPlayerCharacter* AICharacter = Cast<ATagPlayerCharacter>(AIController->GetPawn());
	if (!AICharacter) { return; }
	//bool AmIIt = GameMode->GetItPlayer() == AICharacter;

	for (ATagPlayerCharacter* Character : TActorRange<ATagPlayerCharacter>(GetWorld()))
	{
		// Don't Add itself to the list
		if (Character != AICharacter)
		{
			Players.Add(Character);
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Players[FMath::RandRange(0, Players.Num() - 1)]);
}
