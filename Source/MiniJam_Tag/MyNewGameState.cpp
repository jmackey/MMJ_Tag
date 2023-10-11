// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewGameState.h"
#include "Net/UnrealNetwork.h"

AMyNewGameState::AMyNewGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	GameOver = false;
}

void AMyNewGameState::SetItPlayer(ATagPlayerCharacter* NewItPlayer)
{
	if (!HasAuthority()) { return; }
	if (CurrentCooldown <= 0)
	{
		ItPlayer = NewItPlayer;
		CurrentlyITPlayer = NewItPlayer;
		OnRep_CurrentITPlayer();
		CurrentCooldown = TagCoolDown;
		UE_LOG(LogTemp, Warning, TEXT("GameState: Setting new IT Player"));
	}
}

void AMyNewGameState::OnRep_CurrentITPlayer()
{

}

void AMyNewGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyNewGameState, CurrentlyITPlayer);
	DOREPLIFETIME(AMyNewGameState, GameTime);
}

void AMyNewGameState::OnRep_GameTime()
{
}

void AMyNewGameState::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("GameState: BEGIN PLAY!"));
}

void AMyNewGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("GameState: Tick!"));
	if (!HasAuthority())
	{
		// TODO: This should actually be a client RPC
		if (GameOver) { return; }
		if (GameTime <= 0)
		{
			OnGameOver.Broadcast();
			GameOver = true;
		}
		return;
	}
	if (GameOver) { return; }

	if (GameTime > 0)
	{
		float NewTime = GameTime -= DeltaTime;
		GameTime = FMath::CeilToInt(NewTime);
	}
	else
	{
		GameOver = true;
		OnGameOver.Broadcast();
	}

	if (CurrentCooldown > 0)
	{
		CurrentCooldown -= DeltaTime;
	}
}
