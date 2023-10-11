// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdatePlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TagGameState.h"
#include "TagPlayerCharacter.h"

UBTS_UpdatePlayerLocation::UBTS_UpdatePlayerLocation()
{
	NodeName = TEXT("Update Player Location");
}

void UBTS_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// TODO: Instead of player 0, get the "IT" player
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ATagGameState* TagGameState = Cast<ATagGameState>(UGameplayStatics::GetGameState(this));
	if (!TagGameState) { return; }
	ATagPlayerCharacter* ITPlayer = TagGameState->GetItPlayer();

	if (!ITPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("NO PLAYER PAWN FOUND!"));
		return;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), ITPlayer);
}
