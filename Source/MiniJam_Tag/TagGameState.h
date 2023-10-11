// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TagGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOver);

class ATagPlayerCharacter;

/**
 * 
 */
UCLASS()
class MINIJAM_TAG_API ATagGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATagGameState();
	void Tick(float DeltaTime) override;
	void SetItPlayer(ATagPlayerCharacter* NewItPlayer);
	UFUNCTION(BlueprintCallable)
	ATagPlayerCharacter* GetItPlayer() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentITPlayer, BlueprintReadWrite, VisibleAnywhere)
	ATagPlayerCharacter* CurrentlyITPlayer;

	UFUNCTION()
	void OnRep_CurrentITPlayer();

	UPROPERTY(BlueprintAssignable)
	FGameOver OnGameOver;

	UPROPERTY(ReplicatedUsing = OnRep_GameTime, BlueprintReadWrite, EditAnywhere)
	int GameTime;

	UFUNCTION()
	void OnRep_GameTime();

protected:
	UPROPERTY(EditAnywhere)
	float TagCoolDown = 5.f;
private:
	float CurrentCooldown = 0.f;
	UPROPERTY(VisibleAnywhere)
	ATagPlayerCharacter* ItPlayer = 0;

	bool GameOver = false;
};
