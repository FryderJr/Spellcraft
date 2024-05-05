// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicEffect.h"

void UMagicEffect::Execute(TFunction<void()> OnCommandCompleted)
{
	CommandCompletedCallback = OnCommandCompleted;
}
