// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TagPlayerCharacter.h"
#include "TagGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOver);

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
	UFUNCTION(BlueprintCallable)
	int GetTimeLeft();

protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere)
	ATagPlayerCharacter* ItPlayer;

	UPROPERTY(EditDefaultsOnly)
	float TagCooldown;

	UPROPERTY(VisibleAnywhere)
	float CurrentCooldown;

	UPROPERTY(EditDefaultsOnly)
	int GameTime;

	UPROPERTY(VisibleAnywhere)
	bool GameOver;

	UPROPERTY(BlueprintAssignable)
	FGameOver OnGameOver;
	
};
