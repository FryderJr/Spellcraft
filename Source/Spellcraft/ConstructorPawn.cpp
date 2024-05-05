// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructorPawn.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/SpringArmComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)

// Sets default values
AConstructorPawn::AConstructorPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 2000.0f;
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

}

// Called when the game starts or when spawned
void AConstructorPawn::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	TargetLocation = GetActorLocation();

	TargetZoom = 3000.0f;

	const FRotator Rotation = CameraBoom->GetRelativeRotation();

	TargetRotation = FRotator(Rotation.Pitch + -50.0f, Rotation.Yaw, 0.0f);
}

void AConstructorPawn::MoveTriggered(const FInputActionInstance& Instance)
{
	FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

	Move(AxisValue);
}

void AConstructorPawn::Move(FVector2D Axis)
{
	const FVector ForwardVector = FVector::VectorPlaneProject(CameraBoom->GetForwardVector(), FVector(0, 0, 1.0f));

	TargetLocation += (ForwardVector * Axis.Y + CameraBoom->GetRightVector() * Axis.X) * MoveSpeed;
}

void AConstructorPawn::Zoom(const FInputActionInstance& Instance)
{
	const float ZoomValue = Instance.GetValue().Get<float>() * 100.0f;

	TargetZoom += ZoomValue;

	TargetZoom = FMath::Clamp(FMath::Abs(TargetZoom), MinZoom, MaxZoom);
}

void AConstructorPawn::EdgeScroll()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	MousePosition *= UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	MousePosition.X /= ViewportSize.X;
	MousePosition.Y /= ViewportSize.Y;

	FVector2D Input;

	if (MousePosition.X > 0.9f && MousePosition.X < 1.0f)
	{
		Input.X = 1.0f;
	}

	if (MousePosition.X < 0.1f && MousePosition.X > 0.0f)
	{
		Input.X = -1.0f;
	}

	if (MousePosition.Y > 0.9f && MousePosition.Y < 1.0f)
	{
		Input.Y = -1.0f;
	}

	if (MousePosition.Y < 0.1f && MousePosition.Y > 0.0f)
	{
		Input.Y = 1.0f;
	}

	Move(Input);
}

// Called every frame
void AConstructorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EdgeScroll();

	const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(InterpolatedLocation);

	const float InterpolatedZoom = UKismetMathLibrary::FInterpTo(CameraBoom->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	CameraBoom->TargetArmLength = InterpolatedZoom;

	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(CameraBoom->GetRelativeRotation(), TargetRotation, DeltaTime, RotateSpeed);
	CameraBoom->SetRelativeRotation(InterpolatedRotation);
}

// Called to bind functionality to input
void AConstructorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AConstructorPawn::MoveTriggered);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AConstructorPawn::Zoom);
	}
	else
	{

	}
}

