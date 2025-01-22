// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellcraftConnection.generated.h"

UCLASS()
class SPELLCRAFT_API ASpellcraftConnection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellcraftConnection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spellcraft, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spellcraft, meta = (AllowPrivateAccess = "true"))
	class USplineMeshComponent* SplineMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateConnection(AActor* NodeA, AActor* NodeB);

};
