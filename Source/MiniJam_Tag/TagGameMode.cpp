// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"
#include "EngineUtils.h"
#include "TagPlayerCharacter.h"
#include "TagGameState.h"
#include "Kismet/GameplayStatics.h"

ATagGameMode::ATagGameMode()
{
	//PrimaryActorTick.bCanEverTick = true;
	//GameTime = 20;
	GameOver = false;
}

void ATagGameMode::BeginPlay()
{
	if (!HasAuthority()) { return; }
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
	if (!HasAuthority()) { return; }

	UE_LOG(LogTemp, Display, TEXT("Setting new active player - %s"), *NewItPlayer->GetName());
	ATagGameState* TagGameState = Cast<ATagGameState>(UGameplayStatics::GetGameState(this));
	if (!TagGameState) { return; }
	TagGameState->SetItPlayer(NewItPlayer);
}
