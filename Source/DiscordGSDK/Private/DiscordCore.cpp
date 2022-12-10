// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordCore.h"

#include "DiscordActivityManager.h"
#include "DiscordRelationshipManager.h"
#include "DiscordUserManager.h"
#include "ThirdParty/DiscordGSDKLibrary/Include/core.h"

bool UDiscordCore::Create(int64 ClientID, bool bIsDiscordRequired)
{
	UE_LOG(LogDiscord, Log, TEXT("Trying to create Discord Core..."));

	const discord::Result Result = discord::Core::Create(ClientID, bIsDiscordRequired ? DiscordCreateFlags_Default : DiscordCreateFlags_NoRequireDiscord, &Core);

	if (Result == discord::Result::Ok && Core != nullptr)
	{
		UE_LOG(LogDiscord, Log, TEXT("Discord Core was successfully created."));
		auto LogHook = [=](discord::LogLevel inMinLevel, const char* inLogText)
		{
			OnDiscordLogHook.Broadcast(static_cast<FDiscordLogLevel::Type>(inMinLevel), UTF8_TO_TCHAR(inLogText));
		};
	
		Core->SetLogHook(discord::LogLevel::Debug, LogHook);

		InitializeInterfaces();
	}
	else
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to create Discord Core."));
	}
	
	return Result == discord::Result::Ok;
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
