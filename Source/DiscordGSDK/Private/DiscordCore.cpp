// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordCore.h"
#include "DiscordActivityManager.h"
#include "DiscordGameInstanceSubsystem.h"
#include "DiscordPluginSettings.h"
#include "DiscordRelationshipManager.h"
#include "DiscordUserManager.h"
#include "ThirdParty/DiscordGSDKLibrary/Include/core.h"

UDiscordCore* UDiscordCore::Get(UWorld* World)
{
	if (World
		&& World->GetGameInstance()
		&& World->GetGameInstance()->GetSubsystem<UDiscordGameInstanceSubsystem>()
		&& World->GetGameInstance()->GetSubsystem<UDiscordGameInstanceSubsystem>()->GetDiscordCore())
	{
		return World->GetGameInstance()->GetSubsystem<UDiscordGameInstanceSubsystem>()->GetDiscordCore();
	}
	
	return nullptr;
}

void UDiscordCore::BeginPlay()
{
	if (GetOuter()
		&& Cast<UGameInstance>(GetOuter())
		&& Cast<UGameInstance>(GetOuter())->GetSubsystem<UDiscordGameInstanceSubsystem>()
		&& Cast<UGameInstance>(GetOuter())->GetSubsystem<UDiscordGameInstanceSubsystem>()->GetDiscordCore()
		&& Cast<UGameInstance>(GetOuter())->GetSubsystem<UDiscordGameInstanceSubsystem>()->GetDiscordCore() == this)
	{
		if (ReconnectCount <= 0)
		{
			UE_LOG(LogDiscord, Log, TEXT("Trying to connect with Discord..."));
		}

		const UDiscordPluginSettings* DiscordPluginSettings = GetDefault<UDiscordPluginSettings>();
#if PLATFORM_DESKTOP
		const discord::Result Result = discord::Core::Create(DiscordPluginSettings->ClientID, DiscordCreateFlags_Default, &Core);
#elif
		const discord::Result Result = discord::Core::Create(DiscordPluginSettings->ClientID, DiscordCreateFlags_NoRequireDiscord, &Core);
#endif

		if (Result == discord::Result::Ok && Core != nullptr)
		{
			UE_LOG(LogDiscord, Log, TEXT("Successfully connected with Discord."));
			auto LogHook = [=](discord::LogLevel inMinLevel, const char* inLogText)
			{
				OnDiscordLogHook.Broadcast(static_cast<FDiscordLogLevel::Type>(inMinLevel), UTF8_TO_TCHAR(inLogText));
			};
	
			Core->SetLogHook(static_cast<discord::LogLevel>(DiscordPluginSettings->LogVerbosity.GetValue()), LogHook);

			InitializeInterfaces();
			
			Core->ActivityManager().RegisterSteam(DiscordPluginSettings->SteamAppID);
		}
		else
		{
			// This should be bad. Try to reconnect with Discord if we haven't exceeded out limit.
			if (ReconnectCount < DiscordPluginSettings->ReconnectLimit)
			{
				ReconnectCount++;
				
				FTimerHandle UnusedHandle;
				GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UDiscordCore::BeginPlay, 1, false);
			}
			else
			{
				UE_LOG(LogDiscord, Error, TEXT("Failed to connect with Discord."));
			}
		}
	}
	else
	{
		// We don't want to another DiscordCore object
		MarkAsGarbage();
	}
}

void UDiscordCore::PostInitProperties()
{
	UObject::PostInitProperties();

	if (GetWorld())
	{
		BeginPlay();
	}
}

void UDiscordCore::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (Core)
	{
		delete Core;
		Core = nullptr;
	}
}

UWorld* UDiscordCore::GetWorld() const
{
	// Return pointer to World from object owner, if we don’t work in editor
	if (GIsEditor && !GIsPlayInEditorWorld)
	{
		return nullptr;
	}
	if (GetOuter())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}

void UDiscordCore::Tick(float DeltaTime)
{
	Core->RunCallbacks();
}

bool UDiscordCore::IsTickable() const
{
	return Core != nullptr;
}

TStatId UDiscordCore::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncDiscordHelperCallbackHandler, STATGROUP_ThreadPoolAsyncTasks);
}

void UDiscordCore::InitializeInterfaces()
{
	// @TODO:: All Interfaces.

	// UserManager Interface:
	DiscordUserManager = NewObject<UDiscordUserManager>(this, UDiscordUserManager::StaticClass());
	DiscordUserManager->Create(this);

	// ActivityManager Interface:
	DiscordActivityManager = NewObject<UDiscordActivityManager>(this, UDiscordActivityManager::StaticClass());
	DiscordActivityManager->Create(this);

	// RelationshipManager Interface.
	DiscordRelationshipManager = NewObject<UDiscordRelationshipManager>(this, UDiscordRelationshipManager::StaticClass());
	DiscordRelationshipManager->Create(this);
}
