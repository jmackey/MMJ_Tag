// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveAway.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


UBTT_MoveAway::UBTT_MoveAway()
{
	NodeName = TEXT("Move Away");
}

void UBTT_MoveAway::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UObject* ItPlayer = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("ITPlayer"));
	if (!ItPlayer)
	{
		return;
	}
	AActor* ItPawn = Cast<AActor>(ItPlayer);
	if (!ItPawn)
	{
		return;
	}
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
	{
		return;
	}

	FVector MoveDirection = AIPawn->GetActorLocation() - ItPawn->GetActorLocation();
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), MoveDirection);
}

