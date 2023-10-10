// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TagPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINIJAM_TAG_API ATagPlayerController : public APlayerController
{
	GENERATED_BODY()
	ATagPlayerController();

public:
	UPROPERTY(EditAnywhere)
	class ACameraActor* LevelCamera;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
