// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellcraftPawn.h"
#include "InputActionValue.h"
#include "Magic/SpellcraftNode.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/SpringArmComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)

// Sets default values
ASpellcraftPawn::ASpellcraftPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->TargetArmLength = 400.0f; // Default length
    CameraBoom->bEnableCameraRotationLag = true;
    CameraBoom->CameraRotationLagSpeed = 10.0f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void ASpellcraftPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsMoving && TargetNode)
    {
        // Плавное перемещение камеры к целевой точке
        FVector NewLocation = FMath::VInterpTo(CameraBoom->GetComponentLocation(), CurrentFocusPoint, DeltaTime, MoveSpeed);

        CameraBoom->SetWorldLocation(NewLocation);

        // Проверка, достигла ли камера целевой позиции
        if (FVector::Dist(NewLocation, CurrentFocusPoint) < 10.0f) // Порог для остановки движения
        {
            bIsMoving = false;
        }
    }
}

// Called when the game starts or when spawned
void ASpellcraftPawn::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    //Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void ASpellcraftPawn::MoveCamera(const FInputActionInstance& Instance)
{
    if (!TargetNode) return;

    FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

    // Вращение вокруг целевого объекта
    // Расчет изменения угла поворота для камеры на основе ввода от мыши
    FRotator NewRotation = CameraBoom->GetComponentRotation();
    NewRotation.Yaw -= AxisValue.X * RotationSpeed; // Горизонтальное вращение
    NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch - AxisValue.Y * RotationSpeed, -80.0f, 80.0f); // Вертикальное вращение

    // Применение нового поворота к SpringArm, учитывая текущее положение целевого объекта
    CameraBoom->SetWorldRotation(NewRotation);

    // Поддержание камеры направленной на целевой объект
    FVector CameraLocation = CameraBoom->GetComponentLocation();
    FVector TargetLocation = TargetNode->GetActorLocation();
    FVector Direction = TargetLocation - CameraLocation;
    CameraBoom->SetWorldLocation(TargetLocation - Direction);
}

void ASpellcraftPawn::ZoomCamera(const FInputActionInstance& Instance)
{
    float AxisValue = Instance.GetValue().Get<float>();
    CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - AxisValue * 50.0f, 20.0f, 400.0f);
}

void ASpellcraftPawn::SelectNode(const FInputActionInstance& Instance)
{
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
    {
        return;
    }

    print(FString::Printf(TEXT("Select node")));

    FVector2D MousePosition;
    if (PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        FHitResult HitResult;
        PlayerController->GetHitResultAtScreenPosition(MousePosition, ECC_Camera, true, HitResult);

        // Проверяем, попали ли мы в ноду
        if (HitResult.GetActor())
        {
            print(FString::Printf(TEXT("Select node %s"), *HitResult.GetActor()->GetName()));
            ASpellcraftNode* HitNode = Cast<ASpellcraftNode>(HitResult.GetActor());
            if (HitNode)
            {
                // Устанавливаем эту ноду как целевую для фокуса
                TargetNode = HitNode;
                OnNodeFocus(HitNode);
                FocusOnNode();
            }

        }
    }
}

void ASpellcraftPawn::FocusOnNode()
{
    if (TargetNode)
    {
        CurrentFocusPoint = TargetNode->GetActorLocation();
        bIsMoving = true;
    }
}

// Called to bind functionality to input
void ASpellcraftPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpellcraftPawn::MoveCamera);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpellcraftPawn::MoveCamera);
        EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ASpellcraftPawn::ZoomCamera);
        EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ASpellcraftPawn::SelectNode);
    }
}

