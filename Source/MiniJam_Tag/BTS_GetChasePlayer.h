// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_GetChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class MINIJAM_TAG_API UBTS_GetChasePlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_GetChasePlayer();
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
