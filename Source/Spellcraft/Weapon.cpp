// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Engine/StaticMeshSocket.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform AWeapon::GetWeaponSecondaryHandle()
{
	if (WeaponMesh == nullptr) return FTransform();

	return WeaponMesh->GetSocketTransform(FName("LeftHandHandle"), ERelativeTransformSpace::RTS_World);;
}
