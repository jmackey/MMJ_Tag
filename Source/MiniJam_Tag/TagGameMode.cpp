// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"
#include "EngineUtils.h"
#include "TagPlayerCharacter.h"
#include "MyNewGamestate.h"
#include "Kismet/GameplayStatics.h"

ATagGameMode::ATagGameMode()
{
	bStartPlayersAsSpectators = true;
	bDelayedStart = true;
	GameOver = false;
}

void ATagGameMode::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("GameMode: BEGIN PLAY!"));
	// Begin play on the game mode is called even during delayed start
	// Calling startMatch() calls this begin play again
}

void ATagGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	/*while (GameStateReady != true) 
	{
		UE_LOG(LogTemp, Error, TEXT("Waiting for gamestate"));
	}

	UE_LOG(LogTemp, Error, TEXT("GameState ready!"));*/

}

void ATagGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	TArray<ATagPlayerCharacter*> ActivePlayers;
	// TODO: Randomize initial IT player
	for (ATagPlayerCharacter* Character : TActorRange<ATagPlayerCharacter>(GetWorld()))
	{
		ActivePlayers.Add(Character);
	}
	UE_LOG(LogTemp, Error, TEXT("Has Game Started? %s"), HasMatchStarted() ? TEXT("true") : TEXT("False"));
	SetItPlayer(ActivePlayers[FMath::RandRange(0, ActivePlayers.Num() - 1)]);
}

void ATagGameMode::SetItPlayer(ATagPlayerCharacter* NewItPlayer)
{
	UE_LOG(LogTemp, Display, TEXT("Setting new active player - %s"), *NewItPlayer->GetName());
	AMyNewGameState* TagGameState = Cast<AMyNewGameState>(UGameplayStatics::GetGameState(this));
	if (!TagGameState) { return; }
	TagGameState->SetItPlayer(NewItPlayer);
}

void ATagGameMode::PostLogin(APlayerController* NewPlayer)
{
	ConnectedPlayers.Add(NewPlayer);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("New Player has joined!"));
	FString Msg = FString::Printf(TEXT("There are %i players"), ConnectedPlayers.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, Msg);
	if (ConnectedPlayers.Num() == 2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("There are 2 players here!"));
		StartPlay();
		StartMatch();
	}
}
