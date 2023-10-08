// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TagPlayerCharacter.h"
#include "TagGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINIJAM_TAG_API ATagGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATagGameMode();
	void SetItPlayer(ATagPlayerCharacter* NewItPlayer);
	UFUNCTION(BlueprintCallable)
	ATagPlayerCharacter* GetItPlayer();

protected:
	void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	ATagPlayerCharacter* ItPlayer;
	
};
