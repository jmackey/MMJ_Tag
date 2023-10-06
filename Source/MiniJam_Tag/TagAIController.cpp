// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void ATagAIController::BeginPlay()
{
	Super::BeginPlay();
	// Run BTree here
	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
	}
}

void ATagAIController::Tick(float DeltaTime)
{
}
