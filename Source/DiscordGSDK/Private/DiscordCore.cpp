// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordCore.h"
#include "DiscordActivityManager.h"
#include "DiscordPluginSettings.h"
#include "DiscordRelationshipManager.h"
#include "DiscordUserManager.h"
#if DISCORD_GAMESDK_DYNAMIC_LIB
#include "DiscordGSDK/ThirdParty/Discord/core.h"
#endif

void UDiscordCore::Initialize(bool bIsDiscordRequired)
{
#if DISCORD_GAMESDK_DYNAMIC_LIB
	if (ReconnectCount <= 0)
	{
		UE_LOG(LogDiscord, Log, TEXT("Trying to connect with Discord..."));
	}

	const UDiscordPluginSettings* DiscordPluginSettings = GetDefault<UDiscordPluginSettings>();
#if PLATFORM_DESKTOP
	const discord::Result Result = discord::Core::Create(DiscordPluginSettings->ClientID, bIsDiscordRequired ? DiscordCreateFlags_Default : DiscordCreateFlags_NoRequireDiscord, &Core);
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
	
		Core->SetLogHook(discord::LogLevel::Info, LogHook);

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
			GetWorld()->GetTimerManager().SetTimer(UnusedHandle, FTimerDelegate::CreateUObject(this, &UDiscordCore::Initialize, bIsDiscordRequired), 1, false);
		}
		else
		{
			UE_LOG(LogDiscord, Error, TEXT("Failed to connect with Discord."));

			MarkAsGarbage();
		}
	}
#else
	UE_LOG(LogDiscord, Warning, TEXT("DiscordGSDK plugin isn't functioning properly due to missing files."));	
#endif
}

void UDiscordCore::BeginDestroy()
{
	Super::BeginDestroy();

#if DISCORD_GAMESDK_DYNAMIC_LIB
	if (Core)
	{
		delete Core;
		Core = nullptr;
	}
#endif
}

void UDiscordCore::Tick(float DeltaTime)
{
#if DISCORD_GAMESDK_DYNAMIC_LIB
	Core->RunCallbacks();
#endif
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
