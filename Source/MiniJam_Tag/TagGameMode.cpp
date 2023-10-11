// Fill out your copyright notice in the Description page of Project Settings.


#include "TagGameMode.h"
#include "EngineUtils.h"
#include "TagPlayerCharacter.h"
#include "TagGameState.h"
#include "Kismet/GameplayStatics.h"

ATagGameMode::ATagGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	TagCooldown = 5.f;
	GameTime = 20;
	GameOver = false;
}

void ATagGameMode::MulticastRPCITPlayerUpdated_Implementation(ATagPlayerCharacter* NewItPlayer)
{
	if (!HasAuthority()) 
	{
		UE_LOG(LogTemp, Error, TEXT("Does this get called on the client? %s"), *NewItPlayer->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Does this get called on the server? %s"), *NewItPlayer->GetName());

	}

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

void ATagGameMode::Tick(float DeltaSeconds)
{
	if (!HasAuthority()) { return; }
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
	if (!HasAuthority()) { return; }
	if (CurrentCooldown > 0)
	{
		//UE_LOG(LogTemp, Display, TEXT("TAG IN COOLDOWN"));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Setting new active player - %s"), *NewItPlayer->GetName());
	ATagGameState* TagGameState = Cast<ATagGameState>(UGameplayStatics::GetGameState(this));
	if (!TagGameState) { return; }
	TagGameState->SetItPlayer(NewItPlayer);
	CurrentCooldown = TagCooldown;
}

//ATagPlayerCharacter* ATagGameMode::GetItPlayer()
//{
//	return ItPlayer;
//}

int ATagGameMode::GetTimeLeft()
{
	return GameTime / 100;
}

