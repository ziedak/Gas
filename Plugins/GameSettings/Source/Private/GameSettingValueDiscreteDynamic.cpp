// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameSettingValueDiscreteDynamic.h"
#include "DataSource/GameSettingDataSource.h"
#include "UObject/WeakObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameSettingValueDiscreteDynamic)

#define LOCTEXT_NAMESPACE "GameSettingValueDiscreteDynamic"

//////////////////////////////////////////////////////////////////////////
// UGameSettingValueDiscreteDynamic
//////////////////////////////////////////////////////////////////////////

void UGameSettingValueDiscreteDynamic::AddDynamicOption(const FString& InOptionValue, const FText& InOptionText)
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!OptionValues.Contains(InOptionValue),
	                 TEXT("You already added this option InOptionValue: %s InOptionText %s."), *InOptionValue,
	                 *InOptionText.ToString());
#endif

	OptionValues.Add(InOptionValue);
	OptionDisplayTexts.Add(InOptionText);
}

void UGameSettingValueDiscreteDynamic::RemoveDynamicOption(const FString& InOptionValue)
{
	const int32 Index = OptionValues.IndexOfByKey(InOptionValue);
	if (Index == INDEX_NONE) return;

	OptionValues.RemoveAt(Index);
	OptionDisplayTexts.RemoveAt(Index);
}

FString UGameSettingValueDiscreteDynamic::GetValueAsString() const
{
	return Getter->GetValueAsString(LocalPlayer);
}

void UGameSettingValueDiscreteDynamic::SetValueFromString(const FString& InStringValue)
{
	SetValueFromString(InStringValue, EGameSettingChangeReason::Change);
}

void UGameSettingValueDiscreteDynamic::SetValueFromString(const FString& InStringValue, EGameSettingChangeReason Reason)
{
	check(Setter);
	Setter->SetValue(LocalPlayer, InStringValue);

	NotifySettingChanged(Reason);
}

void UGameSettingValueDiscreteDynamic::OnInitialized()
{
#if !UE_BUILD_SHIPPING
	ensureAlways(Getter);
	ensureAlwaysMsgf(Getter->Resolve(LocalPlayer),
	                 TEXT(
		                 "%s: %s did not resolve, are all functions and properties valid, and are they UFunctions/UProperties? Does the getter function have no parameters?"
	                 ), *GetDevName().ToString(), *Getter->ToString());
	ensureAlways(Setter);
	ensureAlwaysMsgf(Setter->Resolve(LocalPlayer),
	                 TEXT(
		                 "%s: %s did not resolve, are all functions and properties valid, and are they UFunctions/UProperties? Does the setting function have exactly one parameter?"
	                 ), *GetDevName().ToString(), *Setter->ToString());
#endif

	Super::OnInitialized();
}

void UGameSettingValueDiscreteDynamic::Startup()
{
	// Should I also do something with Setter?
	check(Getter);
	Getter->Startup(LocalPlayer, FSimpleDelegate::CreateUObject(this, &ThisClass::OnDataSourcesReady));
}

void UGameSettingValueDiscreteDynamic::OnDataSourcesReady()
{
	StartupComplete();
}

void UGameSettingValueDiscreteDynamic::StoreInitial()
{
	InitialValue = GetValueAsString();
}

void UGameSettingValueDiscreteDynamic::ResetToDefault()
{
	if (DefaultValue.IsSet())
	{
		SetValueFromString(DefaultValue.GetValue(), EGameSettingChangeReason::ResetToDefault);
	}
}

void UGameSettingValueDiscreteDynamic::RestoreToInitial()
{
	SetValueFromString(InitialValue, EGameSettingChangeReason::RestoreToInitial);
}

void UGameSettingValueDiscreteDynamic::SetDiscreteOptionByIndex(int32 Index)
{
	if (ensure(OptionValues.IsValidIndex(Index)))
	{
		SetValueFromString(OptionValues[Index]);
	}
}

int32 UGameSettingValueDiscreteDynamic::GetDiscreteOptionIndex() const
{
	const FString CurrentValue = GetValueAsString();
	const int32 Index = OptionValues.IndexOfByPredicate([this, CurrentValue](const FString& InOption)
	{
		return AreOptionsEqual(CurrentValue, InOption);
	});

	// If we can't find the correct index, send the default index.
	if (Index == INDEX_NONE)
	{
		return GetDiscreteOptionDefaultIndex();
	}

	return Index;
}

int32 UGameSettingValueDiscreteDynamic::GetDiscreteOptionDefaultIndex() const
{
	return DefaultValue.IsSet()
		       ? OptionValues.IndexOfByPredicate([this](const FString& InOption)
		       {
			       return AreOptionsEqual(DefaultValue.GetValue(), InOption);
		       })
		       : INDEX_NONE;
}

TArray<FText> UGameSettingValueDiscreteDynamic::GetDiscreteOptions() const
{
	const TArray<FString>& DisabledOptions = GetEditState().GetDisabledOptions();

	if (DisabledOptions.Num() <= 0)
		return OptionDisplayTexts;

	TArray<FText> AllowedOptions;
	for (int32 OptionIndex = 0; OptionIndex < OptionValues.Num(); ++OptionIndex)
	{
		if (!DisabledOptions.Contains(OptionValues[OptionIndex]))
		{
			AllowedOptions.Add(OptionDisplayTexts[OptionIndex]);
		}
	}

	return AllowedOptions;
}

//////////////////////////////////////////////////////////////////////////
// UGameSettingValueDiscreteDynamic_Bool
//////////////////////////////////////////////////////////////////////////

UGameSettingValueDiscreteDynamic_Bool* UGameSettingValueDiscreteDynamic_Bool::CreateSettings(const FName& DevName,
	const FText& DisplayName, const FText& Description, const TSharedRef<FGameSettingDataSource>& Getter,
	const TSharedRef<FGameSettingDataSource>& Setter, const bool DefaultValue)

{
	const auto Setting = NewObject<UGameSettingValueDiscreteDynamic_Bool>();
	Setting->SetDevName(DevName);
	Setting->SetDisplayName(DisplayName);
	Setting->SetDescriptionRichText(Description);
	Setting->SetDynamicGetter(Getter);
	Setting->SetDynamicSetter(Setter);
	Setting->SetDefaultValue(DefaultValue);

	return Setting;
}


UGameSettingValueDiscreteDynamic_Bool::UGameSettingValueDiscreteDynamic_Bool()
{
	AddDynamicOption(TEXT("false"), LOCTEXT("OFF", "OFF"));
	AddDynamicOption(TEXT("true"), LOCTEXT("ON", "ON"));
}

void UGameSettingValueDiscreteDynamic_Bool::SetTrueText(const FText& InText)
{
	// We remove and then re-add it, so that by changing the true/false text you can also control the order they appear.
	RemoveDynamicOption(TEXT("true"));
	AddDynamicOption(TEXT("true"), InText);
}

void UGameSettingValueDiscreteDynamic_Bool::SetFalseText(const FText& InText)
{
	// We remove and then re-add it, so that by changing the true/false text you can also control the order they appear.
	RemoveDynamicOption(TEXT("false"));
	AddDynamicOption(TEXT("false"), InText);
}

void UGameSettingValueDiscreteDynamic_Bool::SetDefaultValue(bool Value)
{
	DefaultValue = LexToString(Value);
}

//////////////////////////////////////////////////////////////////////////
// UGameSettingValueDiscreteDynamic_Number
//////////////////////////////////////////////////////////////////////////

void UGameSettingValueDiscreteDynamic_Number::OnInitialized()
{
	Super::OnInitialized();

	ensure(OptionValues.Num() > 0);
}

//////////////////////////////////////////////////////////////////////////
// UGameSettingValueDiscreteDynamic_Enum
//////////////////////////////////////////////////////////////////////////

void UGameSettingValueDiscreteDynamic_Enum::OnInitialized()
{
	Super::OnInitialized();
	ensure(OptionValues.Num() > 0);
}

#undef LOCTEXT_NAMESPACE
