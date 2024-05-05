// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileForm.h"
#include "TargetAroundEffect.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)

AProjectileForm::AProjectileForm()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("CollisionComponent"));
	SphereComponent->SetSphereRadius(14.428404f);
	SphereComponent->SetCollisionProfileName(FName("Projectile"));
	//SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	RootComponent = SphereComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileMesh->SetupAttachment(RootComponent);

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

	if (ProjectileMesh->GetStaticMesh() == nullptr)
	{
		UStaticMesh* ProjectileStaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/Sphere.Sphere'")));
		if (ProjectileStaticMesh != nullptr)
		{
			ProjectileMesh->SetStaticMesh(ProjectileStaticMesh);
			ProjectileMesh->SetWorldScale3D(FVector(0.1, 0.1, 0.1));
			SphereComponent->SetSphereRadius(14.428404);
		}
	}
}

void AProjectileForm::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HitActor = OtherActor;
	HitActorLocation = Hit.Location;

	//print(FString::Printf(TEXT("Projectile hit Actor is %s"), *HitActor->GetName()));

	UTargetAroundEffect* MagicEffect = NewObject<UTargetAroundEffect>(GetWorld());
	MagicEffect->Id = SpellData->OnCollide;
	MagicEffect->TargetCenter = Hit.Location;
	MagicEffect->HitActor = OtherActor;

	MagicEffect->Execute([this] 
		{
			
		}
	);
	SpellDestruction();
}