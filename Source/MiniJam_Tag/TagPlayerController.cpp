// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"

ATagPlayerController::ATagPlayerController()
{


}

void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();
	for (ACameraActor* Camera : TActorRange<ACameraActor>(GetWorld()))
	{
		FViewTargetTransitionParams Params;
		Params.BlendTime = 0;
		AActor* CameraActor = Cast<AActor>(Camera);
		if (!CameraActor)
		{
			UE_LOG(LogTemp, Error, TEXT("Camera can't be cast to actor!"));
			return;
		}
		SetViewTarget(CameraActor, Params);
		break;
	}
}
