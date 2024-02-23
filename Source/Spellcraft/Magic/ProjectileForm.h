// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseForm.h"
#include "ProjectileForm.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

/**
 * 
 */
UCLASS()
class SPELLCRAFT_API AProjectileForm : public ABaseForm
{
	GENERATED_BODY()

public:

	AProjectileForm();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	UParticleSystem* ImpactEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};
