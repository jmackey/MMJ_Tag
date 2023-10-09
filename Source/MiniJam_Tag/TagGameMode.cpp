// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"
#include "EngineUtils.h"
#include "TagPlayerCharacter.h"

ATagGameMode::ATagGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	TagCooldown = 5.f;
	GameTime = 20;
	GameOver = false;
}

void ATagGameMode::BeginPlay()
{
	TArray<ATagPlayerCharacter*> ActivePlayers;
	// TODO: Randomize initial IT player
	for (ATagPlayerCharacter* Character : TActorRange<ATagPlayerCharacter>(GetWorld()))
	{
		ActivePlayers.Add(Character);
	}

	SetItPlayer(ActivePlayers[FMath::RandRange(0, ActivePlayers.Num() - 1)]);
}

void ATagGameMode::Tick(float DeltaSeconds)
{
	if (GameOver)
	{
		return;
	}
	if (CurrentCooldown >= 0)
	{
		CurrentCooldown -= DeltaSeconds;
	}

	if (GetTimeLeft() > 0)
	{
		float NewTime = GameTime -= DeltaSeconds;
		GameTime = FMath::CeilToInt(NewTime);
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("Game Over!"));
		OnGameOver.Broadcast();
		GameOver = true;
	}
}


void ATagGameMode::SetItPlayer(ATagPlayerCharacter* NewItPlayer)
{
	if (CurrentCooldown > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("TAG IN COOLDOWN"));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Setting new active player - %s"), *NewItPlayer->GetName());
	ItPlayer = NewItPlayer;
	// TODO: Broadcast that a new player is IT
	OnTagPlayerChanged.Broadcast();
	CurrentCooldown = TagCooldown;
}

ATagPlayerCharacter* ATagGameMode::GetItPlayer()
{
	return ItPlayer;
}

int ATagGameMode::GetTimeLeft()
{
	return GameTime / 100;
}

