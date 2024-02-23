// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellcraftAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Spellcraft/SpellcraftCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,text)

void USpellcraftAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SpellcraftCharacter = Cast<ASpellcraftCharacter>(TryGetPawnOwner());
}

void USpellcraftAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SpellcraftCharacter == nullptr)
	{
		SpellcraftCharacter = Cast<ASpellcraftCharacter>(TryGetPawnOwner());
	}

	if (SpellcraftCharacter == nullptr) return;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(SpellcraftCharacter->GetMovementComponent());

	if (MovementComponent == nullptr) return;

	bIsInAir = SpellcraftCharacter->GetCharacterMovement()->IsFalling();

	Velocity = SpellcraftCharacter->GetVelocity();
	float Z = Velocity.Z;
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	Velocity.Z = Z;
	bool bAccelerating = UKismetMathLibrary::NotEqual_VectorVector(MovementComponent->GetCurrentAcceleration(), FVector(0), 0.0);
	bShouldMove = bAccelerating && Speed > 3.0;

	FRotator AimRotation = SpellcraftCharacter->GetActorRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(SpellcraftCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	Yaw = DeltaRotation.Yaw;

	RighthandTransform = SpellcraftCharacter->GetWeaponRightHandGrip();
	FVector OutPosition;
	FRotator OutRotation;
	SpellcraftCharacter->GetMesh()->TransformToBoneSpace(FName("hand_l"), RighthandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
	RighthandTransform.SetLocation(OutPosition);
	RighthandTransform.SetRotation(FQuat(OutRotation));
}
