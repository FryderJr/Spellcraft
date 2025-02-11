// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpellcraftCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Magic/ProjectileForm.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Weapon.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)

ASpellcraftCharacter::ASpellcraftCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 240.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ASpellcraftCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ASpellcraftCharacter::BeginPlay()
{
	Super::BeginPlay();

	LastCombo = 0;

	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponType, FActorSpawnParameters());

	Controller = Cast<APlayerController>(GetController());

	AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->OnMontageEnded.AddDynamic(this, &ASpellcraftCharacter::AttackAnimationEnd);

	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASpellcraftCharacter::AnimNotifyBegin);

	//AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASpellcraftCharacter::AttackAnimationEnd);

	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("Weapon_L"));
	if (HandSocket)
	{
		HandSocket->AttachActor(Weapon, GetMesh());
	}

	RightHandSocket = GetMesh()->GetSocketByName(FName("Weapon_R"));
}

FTransform ASpellcraftCharacter::GetWeaponRightHandGrip()
{
	FTransform SocketTransform;

	if (Weapon == nullptr) return SocketTransform;

	return Weapon->GetWeaponSecondaryHandle();
}

void ASpellcraftCharacter::Attack()
{
	if (AnimInstance && CastMagicAnim)
	{
		AnimInstance->Montage_Play(CastMagicAnim, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f);
		DisableInput(Controller);
	}
}

void ASpellcraftCharacter::AttackAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	LastTimeCombo = FPlatformTime::Seconds();

	if (!InputEnabled())
	{
		EnableInput(Controller);
	}
}

void ASpellcraftCharacter::AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("CastMagic"))
	{
		FTransform SpellTransform;
		SpellTransform.SetLocation(RightHandSocket->GetSocketLocation(GetMesh()));
		SpellTransform.SetRotation(GetActorRotation().Quaternion());
		AProjectileForm* Spell = GetWorld()->SpawnActorDeferred<AProjectileForm>(AProjectileForm::StaticClass(), SpellTransform);
		Spell->Id = 0;
		Spell->FinishSpawning(SpellTransform);
	}
}
