// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_ITService.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TagGameMode.h"
#include "AIController.h"
#include "TagPlayerCharacter.h"


UBTS_ITService::UBTS_ITService()
{
	NodeName = TEXT("Am I It?");
}

void UBTS_ITService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ATagGameMode* GameMode = Cast<ATagGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode) 
	{
		return;
	}
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) { return; }

	ATagPlayerCharacter* AICharacter = Cast<ATagPlayerCharacter>(AIController->GetPawn());
	if (!AICharacter) { return; }
	bool AmIIt = GameMode->GetItPlayer() == AICharacter;

	if (AmIIt) 
	{
		UE_LOG(LogTemp, Warning, TEXT("I AM IT!"));
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), AmIIt);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I AM NOT IT!"));
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}

}