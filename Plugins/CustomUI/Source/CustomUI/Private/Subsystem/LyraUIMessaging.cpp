// Copyright Epic Games, Inc. All Rights Reserved.

#include "Subsystem/LyraUIMessaging.h"

#include "Messaging/CommonGameDialog.h"
#include "NativeGameplayTags.h"
#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraUIMessaging)

class FSubsystemCollectionBase;

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MODAL, "UI.Layer.Modal");

void ULyraUIMessaging::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ConfirmationDialogClassPtr = ConfirmationDialogClass.LoadSynchronous();
	ErrorDialogClassPtr = ErrorDialogClass.LoadSynchronous();
}

void ULyraUIMessaging::ShowConfirmation(UCommonGameDialogDescriptor* DialogDescriptor,
                                        FCommonMessagingResultDelegate ResultCallback)
{
	if (UCommonLocalPlayer* LocalPlayer = GetLocalPlayer<UCommonLocalPlayer>())
	{
		if (UPrimaryGameLayout* RootLayout = LocalPlayer->GetRootUILayout())
		{
			RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(TAG_UI_LAYER_MODAL, ConfirmationDialogClassPtr,
			                                                      [DialogDescriptor, ResultCallback](
			                                                      UCommonGameDialog& Dialog){
				                                                      Dialog.SetupDialog(
					                                                      DialogDescriptor, ResultCallback);
			                                                      });
		}
	}
}

void ULyraUIMessaging::ShowError(UCommonGameDialogDescriptor* DialogDescriptor,
                                 FCommonMessagingResultDelegate ResultCallback)
{
	if (UCommonLocalPlayer* LocalPlayer = GetLocalPlayer<UCommonLocalPlayer>())
	{
		if (UPrimaryGameLayout* RootLayout = LocalPlayer->GetRootUILayout())
		{
			RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(TAG_UI_LAYER_MODAL, ErrorDialogClassPtr,
			                                                      [DialogDescriptor, ResultCallback](
			                                                      UCommonGameDialog& Dialog){
				                                                      Dialog.SetupDialog(
					                                                      DialogDescriptor, ResultCallback);
			                                                      });
		}
	}
}
