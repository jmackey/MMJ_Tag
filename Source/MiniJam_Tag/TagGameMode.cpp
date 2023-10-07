// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"
#include "EngineUtils.h"
#include "TagPlayerCharacter.h"

ATagGameMode::ATagGameMode()
{
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


void ATagGameMode::SetItPlayer(ATagPlayerCharacter* NewItPlayer)
{
	UE_LOG(LogTemp, Display, TEXT("Setting new active player - %s"), *NewItPlayer->GetName());
	ItPlayer = NewItPlayer;
}

ATagPlayerCharacter* ATagGameMode::GetItPlayer()
{
	return ItPlayer;
}

