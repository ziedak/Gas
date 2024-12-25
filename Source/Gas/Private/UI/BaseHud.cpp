// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseHud.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/GameFrameworkComponentManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseHud)

void ABaseHud::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ABaseHud::BeginPlay()
{
	Super::BeginPlay();
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
		this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

void ABaseHud::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
}

void ABaseHud::GetDebugActorList(TArray<AActor*>& InOutList)
{
	Super::GetDebugActorList(InOutList);
	UWorld* World = GetWorld();
	if (!World) { return; }

	//Add all actors with Asc
	for (TObjectIterator<UAbilitySystemComponent> It; It; ++It)
	{
		const UAbilitySystemComponent* Asc = *It;
		if (!Asc || Asc->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject)) { continue; }

		AActor* AvatarActor = Asc->GetAvatarActor();
		AActor* OwnerActor = Asc->GetOwnerActor();

		if (AvatarActor && UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AvatarActor))
		{
			AddActorToDebugList(AvatarActor, InOutList, World);
		}
		else if (OwnerActor && UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor))
		{
			AddActorToDebugList(OwnerActor, InOutList, World);
		}
	}
}
