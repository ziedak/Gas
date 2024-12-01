// Fill out your copyright notice in the Description page of Project Settings.


#include "Experience/DataAsset/UserFacingExperienceDefinition_DA.h"
#include "CommonSessionSubsystem.h" 

UCommonSession_HostSessionRequest* UUserFacingExperienceDefinition_DA::CreateHostingRequest(
	const UObject* WorldContextObject) const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	const FString UserFacingExperienceName = GetPrimaryAssetId().PrimaryAssetName.ToString();

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	const UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	UCommonSession_HostSessionRequest* Result = nullptr;
	
	if (UCommonSessionSubsystem* Subsystem = GameInstance ? GameInstance->GetSubsystem<UCommonSessionSubsystem>() : nullptr)
	{
		Result = Subsystem->CreateOnlineHostSessionRequest();
	}

	if (!Result)
	{
		// Couldn't use the subsystem so create one
		Result = NewObject<UCommonSession_HostSessionRequest>();
		Result->OnlineMode = ECommonSessionOnlineMode::Online;
		Result->bUseLobbies = true;
	}
	Result->MapID = MapID;
	Result->ModeNameForAdvertisement = UserFacingExperienceName;
	Result->ExtraArgs = ExtraArgs;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);
	Result->MaxPlayerCount = MaxPlayerCount;

	// if (ULyraReplaySubsystem::DoesPlatformSupportReplays())
	// {
	// 	if (bRecordReplay)
	// 	{
	// 		Result->ExtraArgs.Add(TEXT("DemoRec"), FString());
	// 	}
	// }

	return Result;
}
