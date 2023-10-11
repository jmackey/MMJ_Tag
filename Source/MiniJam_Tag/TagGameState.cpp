// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameState.h"

ATagGameState::ATagGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATagGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentCooldown > 0)
	{
		CurrentCooldown -= DeltaTime;
	}
}

void ATagGameState::SetItPlayer(ATagPlayerCharacter* NewItPlayer)
{
	if (!HasAuthority()) { return; }
	if (CurrentCooldown <= 0)
	{
		ItPlayer = NewItPlayer;
		CurrentCooldown = TagCoolDown;
		UE_LOG(LogTemp, Warning, TEXT("GameState: Setting new IT Player"));
	}
}

ATagPlayerCharacter* ATagGameState::GetItPlayer() const
{
	return ItPlayer;
}
