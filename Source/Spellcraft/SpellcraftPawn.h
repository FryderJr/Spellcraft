// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpellcraftPawn.generated.h"

UCLASS()
class SPELLCRAFT_API ASpellcraftPawn : public APawn
{
    GENERATED_BODY()

public:
    ASpellcraftPawn();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
    AActor* TargetNode;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
    float RotationSpeed = 20.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
    float MoveSpeed = 2.0f;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* CameraComponent;

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* ZoomAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* ClickAction;

    FVector CurrentFocusPoint;

    bool bIsMoving;

    UFUNCTION()
    void MoveCamera(const FInputActionInstance& Instance);

    UFUNCTION()
    void ZoomCamera(const FInputActionInstance& Instance);

    UFUNCTION()
    void SelectNode(const FInputActionInstance& Instance);

    UFUNCTION(BlueprintCallable, Category = Spellcraft)
    void FocusOnNode();

public:

    UFUNCTION(BlueprintImplementableEvent, Category = Spellcraft)
    void OnNodeFocus(ASpellcraftNode* Node);
};
