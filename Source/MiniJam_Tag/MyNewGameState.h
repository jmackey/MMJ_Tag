// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TagPlayerCharacter.h"
#include "MyNewGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOver);
/**
 * 
 */
UCLASS()
class MINIJAM_TAG_API AMyNewGameState : public AGameState
{
	GENERATED_BODY()
public:
	AMyNewGameState();
	void SetItPlayer(ATagPlayerCharacter* NewItPlayer);

	UPROPERTY(BlueprintAssignable)
	FGameOver OnGameOver;

	UPROPERTY(ReplicatedUsing = OnRep_GameTime, BlueprintReadWrite, EditAnywhere)
	int GameTime;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentITPlayer, BlueprintReadWrite, VisibleAnywhere)
	ATagPlayerCharacter* CurrentlyITPlayer;

	UFUNCTION()
	void OnRep_CurrentITPlayer();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ATagPlayerCharacter* GetItPlayer();

	UFUNCTION()
	void OnRep_GameTime();
protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	float TagCoolDown = 5.f;
	float CurrentCooldown = 0.f;
	UPROPERTY(VisibleAnywhere)
	ATagPlayerCharacter* ItPlayer = 0;

	bool GameOver = false;
};
