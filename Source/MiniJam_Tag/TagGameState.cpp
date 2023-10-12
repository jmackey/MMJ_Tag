// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "TagGameMode.h"

ATagGameState::ATagGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	GameOver = false;
	GameTime = 60;
}

void ATagGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("GameState: Ticking"));
	if (!HasAuthority()) 
	{ 
		// TODO: This should actually be a client RPC
		if (GameOver) { return; }
		if (GameTime <= 0)
		{
			//OnGameOver.Broadcast();
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
		//OnGameOver.Broadcast();
	}

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
		CurrentlyITPlayer = NewItPlayer;
		OnRep_CurrentITPlayer();
		CurrentCooldown = TagCoolDown;
		UE_LOG(LogTemp, Warning, TEXT("GameState: Setting new IT Player"));
	}
}

ATagPlayerCharacter* ATagGameState::GetItPlayer() const
{
	return CurrentlyITPlayer;
}

void ATagGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATagGameState, CurrentlyITPlayer);
	DOREPLIFETIME(ATagGameState, GameTime);

}

void ATagGameState::OnRep_CurrentITPlayer()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("SERVER: CurrentITPlayer Updated"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CLIENT: CurrentITPlayer Updated"));
	}
}

void ATagGameState::OnRep_GameTime()
{
}

//void ATagGameState::HandleMatchIsWaitingToStart()
//{
//	Super::HandleMatchIsWaitingToStart();
//}

void ATagGameState::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("GameState: BEGIN PLAY!"));
}
