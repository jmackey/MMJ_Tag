// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTT_MoveAway.generated.h"

/**
 * 
 */
UCLASS()
class MINIJAM_TAG_API UBTT_MoveAway : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_MoveAway();
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
