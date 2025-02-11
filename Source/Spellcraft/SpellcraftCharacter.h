// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpellcraftCharacter.generated.h"

UCLASS(Blueprintable)
class ASpellcraftCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpellcraftCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, Category = Weapon)
	FTransform GetWeaponRightHandGrip();

	UFUNCTION(BlueprintCallable, Category = Attack)
	void Attack();

	UFUNCTION(Category = Attack)
	void AttackAnimationEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(Category = Attack)
	void AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	class AWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackComboAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* CastMagicAnim;

	class UAnimInstance* AnimInstance;

	const USkeletalMeshSocket* RightHandSocket;

	APlayerController* Controller;

	double LastTimeCombo;

	int32 LastCombo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TArray<FName> Combos;
};

