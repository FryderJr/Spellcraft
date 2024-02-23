// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpellcraftPlayerController.h"
#include "SpellcraftCharacter.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "SpellcraftCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASpellcraftPlayerController::ASpellcraftPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ASpellcraftPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	RotateToCursor(DeltaTime);
}

void ASpellcraftPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ASpellcraftPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ASpellcraftPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ASpellcraftPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ASpellcraftPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ASpellcraftPlayerController::OnTouchReleased);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpellcraftPlayerController::OnMoveTriggered);
		EnhancedInputComponent->BindAction(Attack, ETriggerEvent::Completed, this, &ASpellcraftPlayerController::OnAttackTriggered);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASpellcraftPlayerController::OnJumpTriggered);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASpellcraftPlayerController::OnSprintStarted);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASpellcraftPlayerController::OnSprintStopped);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASpellcraftPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ASpellcraftPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ASpellcraftPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ASpellcraftPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ASpellcraftPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void ASpellcraftPlayerController::RotateToCursor(float DeltaTime)
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	
	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		APawn* const MyPawn = GetPawn();
		if (MyPawn && MyPawn->InputEnabled())
		{
			FRotator PawnRotation = MyPawn->GetActorRotation();

			MyPawn->SetActorRotation(UKismetMathLibrary::RInterpTo(PawnRotation, FRotator(PawnRotation.Pitch, UKismetMathLibrary::FindLookAtRotation(MyPawn->GetActorLocation(), Hit.ImpactPoint).Yaw, PawnRotation.Roll), DeltaTime, 5));
		}

	}
}

void ASpellcraftPlayerController::OnMoveTriggered(const FInputActionInstance& Instance)
{
	FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

	if (bIsRunning)
	{
		AxisValue.X *= 2.0f;
		AxisValue.Y *= 2.0f;
	}
	
	APawn* const MyPawn = GetPawn();

	if (MyPawn)
	{
		MyPawn->AddMovementInput(MyPawn->GetActorForwardVector() * AxisValue.Y);
		MyPawn->AddMovementInput(MyPawn->GetActorRightVector() * AxisValue.X);
	}

}

void ASpellcraftPlayerController::OnAttackTriggered()
{
	ASpellcraftCharacter* SpellCraftCharacter = Cast<ASpellcraftCharacter>(GetPawn());
	if (SpellCraftCharacter && SpellCraftCharacter->InputEnabled() && !SpellCraftCharacter->GetCharacterMovement()->IsFalling())
	{
		SpellCraftCharacter->Attack();
		SpellCraftCharacter->DisableInput(this);
	}
}

void ASpellcraftPlayerController::OnJumpTriggered()
{
	ASpellcraftCharacter* SpellCraftCharacter = Cast<ASpellcraftCharacter>(GetPawn());
	if (SpellCraftCharacter && SpellCraftCharacter->InputEnabled())
	{
		SpellCraftCharacter->Jump();
	}
}

void ASpellcraftPlayerController::OnSprintStarted()
{
	bIsRunning = true;
}

void ASpellcraftPlayerController::OnSprintStopped()
{
	bIsRunning = false;
}
