// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdatePlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_UpdatePlayerLocation::UBTS_UpdatePlayerLocation()
{
	NodeName = TEXT("Update Player Location");
}

void UBTS_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// TODO: Instead of player 0, get the "IT" player
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("NO PLAYER PAWN FOUND!"));
		return;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
}
