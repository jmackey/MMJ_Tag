// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "TagGameState.h"

// Sets default values
ATagPlayerCharacter::ATagPlayerCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	TagTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TagTrigger"));
	TagTrigger->SetBoxExtent(FVector(60.f, 60.f, 60.f));
	TagTrigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TagTrigger->SetupAttachment(RootComponent);

	ItIdentifier = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IT Identifier"));
	ItIdentifier->SetupAttachment(RootComponent);
	ItIdentifier->SetVisibility(true);

	// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller


	// Create a follow camera
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//void ATagPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ATagPlayerCharacter, AmIIt);
//}
//
//void ATagPlayerCharacter::OnRep_AmIIt()
//{
//	if (AmIIt)
//	{
//		ItIdentifier->SetVisibility(true);
//	}
//	else
//	{
//		ItIdentifier->SetVisibility(false);
//	}
//}



// Called when the game starts or when spawned
void ATagPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//if (!HasAuthority())
	//{
	//	return;
	//}

	//GameMode->OnTagPlayerChanged.AddDynamic(this, &ATagPlayerCharacter::TaggedPlayerChanged);
	//TaggedPlayerChanged();
	
}


void ATagPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//void ATagPlayerCharacter::TaggedPlayerChanged()
//{
//	UE_LOG(LogTemp, Warning, TEXT("TaggedPlayerChanged called on player!"));
//
//	ATagGameMode* GameMode = Cast<ATagGameMode>(UGameplayStatics::GetGameMode(this));
//	if (!GameMode)
//	{
//		return;
//	}
//
//	if (GameMode->GetItPlayer() == this)
//	{
//		AmIIt = true;
//	}
//	else
//	{
//		AmIIt = false;
//	}
//}

// Called every frame
void ATagPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) 
	{ 
		ATagGameState* GameState = Cast<ATagGameState>(UGameplayStatics::GetGameState(this));
		if (!GameState) { return; }
		ATagPlayerCharacter* ITPlayer = GameState->GetItPlayer();
		// TODO: This is not efficient to do every tick but good enough for now
		if (ITPlayer != this)
		{
			ItIdentifier->SetVisibility(false);
		}
		else
		{
			ItIdentifier->SetVisibility(true);
		}
		return; 
	}
	TArray<AActor*> OverlappingActors;
	TagTrigger->GetOverlappingActors(OverlappingActors, ATagPlayerCharacter::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor != this)
		{
			ATagGameState* GameState = Cast<ATagGameState>(UGameplayStatics::GetGameState(this));
			if (!GameState) { return; }
			ATagPlayerCharacter* ITPlayer = GameState->GetItPlayer();

			if (!ITPlayer)
			{
				UE_LOG(LogTemp, Error, TEXT("NO IT PLAYER RETURNED!"))
				return;
			}
			//UE_LOG(LogTemp, Error, TEXT("IT PLAYER IS: %s"), *ITPlayer->GetActorNameOrLabel())

			if (ITPlayer != this && ITPlayer == Actor) 
			{
				//UE_LOG(LogTemp, Warning, TEXT("Someone Tagged me!: %s"), *Actor->GetName());

				/*if (GameMode->GetTagCooldown() <= 0)
				{*/
				//GameMode->SetItPlayer(this);
				//MulticastRPCTagUpdate(this);
				//}
				GameState->SetItPlayer(this);
			}
		}
	}
}

// Called to bind functionality to input
void ATagPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATagPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATagPlayerCharacter::MulticastRPCTagUpdate_Implementation(ATagPlayerCharacter* ItPlayer)
{
	UE_LOG(LogTemp, Error, TEXT("CLIENT RPC IS CALLED"));
}

