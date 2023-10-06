// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TagAIController.generated.h"

/**
 * 
 */
UCLASS()
class MINIJAM_TAG_API ATagAIController : public AAIController
{
	GENERATED_BODY()

	//ATagAIController();

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;
};
