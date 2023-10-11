// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TagPlayerCharacter.h"
#include "TagGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINIJAM_TAG_API ATagGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATagGameMode();
	void SetItPlayer(ATagPlayerCharacter* NewItPlayer);
	void PostLogin(APlayerController* NewPlayer) override;

protected:
	void BeginPlay() override;
	void HandleMatchIsWaitingToStart() override;
	void HandleMatchHasStarted() override;

private:
	UPROPERTY(VisibleAnywhere)
	bool GameOver;
	TArray<APlayerController*> ConnectedPlayers;
};
