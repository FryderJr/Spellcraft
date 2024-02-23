// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileForm.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)

AProjectileForm::AProjectileForm()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("CollisionComponent"));
	SphereComponent->SetSphereRadius(14.428404f);
	SphereComponent->SetCollisionProfileName(TEXT("Projectile"));
	//SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	RootComponent = SphereComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 4000.0f;
	ProjectileMovement->MaxSpeed = 4000.0f;
	ProjectileMovement->bAutoActivate = false;
	//ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AProjectileForm::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->MoveIgnoreActors.Add(ActorToIgnore);
	SphereComponent->OnComponentHit.AddDynamic(this, &AProjectileForm::OnHit);
	ProjectileMovement->Activate(true);
}

void AProjectileForm::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HitActor = OtherActor;
	HitActorLocation = Hit.Location;

	print(FString::Printf(TEXT("Projectile hit Actor is %s"), *HitActor->GetName()));

	StartSpellOverlapCheck(HitActorLocation);
}