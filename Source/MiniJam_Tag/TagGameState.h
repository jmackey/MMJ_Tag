// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TagGameState.generated.h"

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
	ATagPlayerCharacter* GetItPlayer() const;

protected:
	UPROPERTY(EditAnywhere)
	float TagCoolDown = 5.f;
private:
	float CurrentCooldown = 0.f;
	ATagPlayerCharacter* ItPlayer = 0;
};
