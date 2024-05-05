// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ConstructorPawn.generated.h"

UCLASS()
class SPELLCRAFT_API AConstructorPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AConstructorPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float MoveSpeed = 20.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float RotateSpeed = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float RotatePitchMin = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float RotatePitchMax = 80.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float ZoomSpeed = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float MinZoom = 500.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Camera")
	float MaxZoom = 4000.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void MoveTriggered(const FInputActionInstance& Instance);

	void Move(FVector2D Axis);

	UFUNCTION()
	void Zoom(const FInputActionInstance& Instance);

	UFUNCTION()
	void EdgeScroll();

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction;

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY()
	float TargetZoom;

	UPROPERTY()
	bool bCanRotate;

};
