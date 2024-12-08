// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "BaseLocalPlayer.generated.h"

class UInputMappingContext;
struct FSwapAudioOutputResult;
/**
 * 
 */
UCLASS()
class GAS_API UBaseLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()

public:
	//~UObject interface
	virtual void PostInitProperties() override;
	//~End of UObject interface

	//~UPlayer interface
	// virtual void SwitchController(class APlayerController* PC) override;
	// //~End of UPlayer interface

	// //~ULocalPlayer interface
	// virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	// virtual void InitOnlineSession() override;
	//~End of ULocalPlayer interface

	//~IBaseTeamAgentInterface interface
	// virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	// virtual FGenericTeamId GetGenericTeamId() const override;
	// virtual FOnBaseTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IBaseTeamAgentInterface interface

	/** Gets the local settings for this player, this is read from config files at process startup and is always valid */
	// UFUNCTION()
	// UBaseSettingsLocal* GetLocalSettings() const{return UBaseSettingsLocal::Get();}

	/** Gets the shared setting for this player, this is read using the save game system so may not be correct until after user login */
	// UFUNCTION()
	// UBaseSettingsShared* GetSharedSettings() const;

	/** Starts an async request to load the shared settings, this will call OnSharedSettingsLoaded after loading or creating new ones */
	// void LoadSharedSettingsFromDisk(bool bForceLoad = false);

protected:
	// void OnSharedSettingsLoaded(UBaseSettingsShared* LoadedOrCreatedSettings);

	// void OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId);

	// UFUNCTION()
	// void OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult);

	// void OnPlayerControllerChanged(APlayerController* NewController);

	// UFUNCTION()
	// void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	// UPROPERTY(Transient)
	// mutable TObjectPtr<UBaseSettingsShared> SharedSettings;

	// FUniqueNetIdRepl NetIdForSharedSettings;

	// UPROPERTY(Transient)
	// mutable TObjectPtr<const UInputMappingContext> InputMappingContext;

	// // UPROPERTY()
	// // FOnBaseTeamIndexChangedDelegate OnTeamChangedDelegate;

	// UPROPERTY()
	// TWeakObjectPtr<APlayerController> LastBoundPC;
};