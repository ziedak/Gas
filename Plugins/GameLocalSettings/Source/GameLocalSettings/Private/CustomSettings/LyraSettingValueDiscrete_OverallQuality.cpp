// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomSettings/LyraSettingValueDiscrete_OverallQuality.h"

#include "Engine/Engine.h"
#include "Settings/LyraSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraSettingValueDiscrete_OverallQuality)

#define LOCTEXT_NAMESPACE "LyraSettings"

ULyraSettingValueDiscrete_OverallQuality::ULyraSettingValueDiscrete_OverallQuality()
{
}

void ULyraSettingValueDiscrete_OverallQuality::OnInitialized()
{
	Super::OnInitialized();

	const ULyraSettingsLocal* UserSettings = ULyraSettingsLocal::Get();
	const int32 MaxQualityLevel = UserSettings->GetMaxSupportedOverallQualityLevel();

	auto AddOptionIfPossible = [&](const int Index, FText&& Value)
	{
		if ((MaxQualityLevel < 0) || (Index <= MaxQualityLevel)) { Options.Add(Value); }
	};

	AddOptionIfPossible(0, LOCTEXT("VideoQualityOverall_Low", "Low"));
	AddOptionIfPossible(1, LOCTEXT("VideoQualityOverall_Medium", "Medium"));
	AddOptionIfPossible(2, LOCTEXT("VideoQualityOverall_High", "High"));
	AddOptionIfPossible(3, LOCTEXT("VideoQualityOverall_Epic", "Epic"));

	OptionsWithCustom = Options;
	OptionsWithCustom.Add(LOCTEXT("VideoQualityOverall_Custom", "Custom"));

	const int32 LowestQualityWithFrameRateLimit = UserSettings->GetLowestQualityWithFrameRateLimit();
	if (Options.IsValidIndex(LowestQualityWithFrameRateLimit))
	{
		SetWarningRichText(FText::Format(
			LOCTEXT("OverallQuality_Mobile_ImpactsFramerate",
			        "<strong>Note: Changing the Quality setting to {0} or higher might limit your framerate.</>"),
			Options[LowestQualityWithFrameRateLimit]));
	}
}

void ULyraSettingValueDiscrete_OverallQuality::StoreInitial()
{
		
}

void ULyraSettingValueDiscrete_OverallQuality::ResetToDefault()
{
}

void ULyraSettingValueDiscrete_OverallQuality::RestoreToInitial()
{
}

void ULyraSettingValueDiscrete_OverallQuality::SetDiscreteOptionByIndex(int32 Index)
{
	UGameUserSettings* UserSettings = CastChecked<UGameUserSettings>(GEngine->GetGameUserSettings());

		// Low / Medium / High / Epic
	if (Index != GetCustomOptionIndex())
		UserSettings->SetOverallScalabilityLevel(Index);
		// Leave everything as is we're in a custom setup.
	NotifySettingChanged(EGameSettingChangeReason::Change);
}

int32 ULyraSettingValueDiscrete_OverallQuality::GetDiscreteOptionIndex() const
{
	const int32 OverallQualityLevel = GetOverallQualityLevel();
	return OverallQualityLevel == INDEX_NONE ? GetCustomOptionIndex() : OverallQualityLevel;
}

TArray<FText> ULyraSettingValueDiscrete_OverallQuality::GetDiscreteOptions() const
{
	return GetOverallQualityLevel() == INDEX_NONE ? OptionsWithCustom : Options;
}

int32 ULyraSettingValueDiscrete_OverallQuality::GetCustomOptionIndex() const
{
	return OptionsWithCustom.Num() - 1;
}

int32 ULyraSettingValueDiscrete_OverallQuality::GetOverallQualityLevel() const
{
	const UGameUserSettings* UserSettings = CastChecked<const UGameUserSettings>(GEngine->GetGameUserSettings());
	return UserSettings->GetOverallScalabilityLevel();
}

#undef LOCTEXT_NAMESPACE
