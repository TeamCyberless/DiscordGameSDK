// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordCore.h"
#include "ThirdParty/DiscordGSDKLibrary/Include/core.h"

TEnumAsByte<FDiscordResult::Type> UDiscordCore::Create(int64 ClientID, bool bIsDiscordRequired)
{
	UE_LOG(LogDiscord, Log, TEXT("Trying to create Discord Core..."));

	const FDiscordResult::Type Result = static_cast<FDiscordResult::Type>(discord::Core::Create(ClientID, bIsDiscordRequired ? DiscordCreateFlags_Default : DiscordCreateFlags_NoRequireDiscord, &Core));

	if (Result == FDiscordResult::Ok && Core != nullptr)
	{
		auto LogHook = [=](discord::LogLevel inMinLevel, const char* inLogText)
		{
			OnDiscordLogHook.Broadcast(static_cast<FDiscordLogLevel::Type>(inMinLevel), UTF8_TO_TCHAR(inLogText));
		};
	
		Core->SetLogHook(discord::LogLevel::Debug, LogHook);
	}
	else
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to create Discord Core."));
	}

	return Result;
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
