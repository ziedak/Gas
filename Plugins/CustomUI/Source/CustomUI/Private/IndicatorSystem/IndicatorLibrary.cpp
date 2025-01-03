// Copyright Epic Games, Inc. All Rights Reserved.

#include "IndicatorSystem/IndicatorLibrary.h"

#include "IndicatorSystem/LyraIndicatorManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IndicatorLibrary)

class AController;

UIndicatorLibrary::UIndicatorLibrary() {}

ULyraIndicatorManagerComponent* UIndicatorLibrary::GetIndicatorManagerComponent(AController* Controller)
{
	return ULyraIndicatorManagerComponent::GetComponent(Controller);
}
