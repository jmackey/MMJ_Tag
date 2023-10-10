// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TagGameMode.h"

ATagPlayerController::ATagPlayerController()
{


}


void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();
	/*if (!HasAuthority()) 
	{
		return;
	}
	
	HUD = CreateWidget(this, HUDClass);
	if (!HUD)
	{
		return;

	}
	HUD->AddToViewport();

	ATagGameMode* GameMode = Cast<ATagGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("No Game Mode!"));
		return;
	}

	GameMode->OnGameOver.AddDynamic(this, &ATagPlayerController::GameOver);*/

	//for (ACameraActor* Camera : TActorRange<ACameraActor>(GetWorld()))
	//{
	//	if (!Camera->ActorHasTag("LevelCamera"))
	//	{
	//		continue;
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("Found a camera!"));
	//	FViewTargetTransitionParams Params;
	//	Params.BlendTime = 0;
	//	AActor* CameraActor = Cast<AActor>(Camera);
	//	if (!CameraActor)
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Camera can't be cast to actor!"));
	//		return;
	//	}
	//	SetViewTarget(CameraActor, Params);
	//	break;
	//}
}

void ATagPlayerController::GameOver()
{
	HUD->RemoveFromViewport();

}


