// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiscordHelper.h"
#include "DiscordLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiscord, All, All)

bool UDiscordHelper::Initialize(int64 ClientID, bool bDiscordRequired)
{
	UE_LOG(LogDiscord, Log, TEXT("Trying to initialize Discord Instance..."));

	const discord::Result Result = discord::Core::Create(ClientID, bDiscordRequired ? DiscordCreateFlags_Default : DiscordCreateFlags_NoRequireDiscord, &Core);
	if (Result != discord::Result::Ok || Core == nullptr)
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to create Discord Instance"));
		return false;
	}

	auto LogHook = [](discord::LogLevel DiscordLogLevel, const char* LogText)
	{
		if (DiscordLogLevel == discord::LogLevel::Error)
		{
			UE_LOG(LogDiscord, Error, TEXT("%s"), ANSI_TO_TCHAR(LogText));
		}
		else if (DiscordLogLevel == discord::LogLevel::Warn)
		{
			UE_LOG(LogDiscord, Warning, TEXT("%s"), ANSI_TO_TCHAR(LogText));
		}
		else
		{
			UE_LOG(LogDiscord, Log, TEXT("%s"), ANSI_TO_TCHAR(LogText));
		}
	};

	// NOTE: LOG HOOK IS CURRENTLY NOT WORKING
	Core->SetLogHook(discord::LogLevel::Debug, LogHook);

	discord::Event<> OnUserConnectedEvent;
	auto UserConnectionHandler = [&]()
	{
		if (Core)
		{
			// BUG: After 3 consecutive connect/disconnect requests, discord stops giving reply to user connection request.
			// Only solution is restarting the Discord. Probably they have a rate limiting.
			discord::User CurrentUser;
			const discord::Result UserResult = Core->UserManager().GetCurrentUser(&CurrentUser);
			if (UserResult == discord::Result::Ok)
			{
				bGotUserConnectedReply = true;
				OnUserConnected.Broadcast(UTF8_TO_TCHAR(CurrentUser.GetUsername()), CurrentUser.GetId(), UTF8_TO_TCHAR(CurrentUser.GetDiscriminator()));
				UE_LOG(LogDiscord, Log, TEXT("Successfully connected to Discord as %s#%s"),
					UTF8_TO_TCHAR(CurrentUser.GetUsername()),
					UTF8_TO_TCHAR(CurrentUser.GetDiscriminator())
				);
			}
		}
	};

	OnUserConnectedEvent.Connect(UserConnectionHandler);
	Core->UserManager().OnCurrentUserUpdate = OnUserConnectedEvent;

	return true;
}

void UDiscordHelper::BeginDestroy()
{
	Super::BeginDestroy();

	if (Core)
	{
		delete Core;
		Core = nullptr;
	}
}

void UDiscordHelper::Tick(float DeltaTime)
{
	Core->RunCallbacks();
}

bool UDiscordHelper::IsTickable() const
{
	return Core != nullptr;
}

TStatId UDiscordHelper::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncDiscordHelperCallbackHandler, STATGROUP_ThreadPoolAsyncTasks);
}

int64 UDiscordHelper::GetConnectedUserID()
{
	if (Core)
	{
		discord::User CurrentUser;
		const discord::Result Result = Core->UserManager().GetCurrentUser(&CurrentUser);
		if (Result == discord::Result::Ok)
		{
			return CurrentUser.GetId();
		}
	}
	return 0;
}

FString UDiscordHelper::GetConnectedUserName()
{
	if (Core)
	{
		discord::User CurrentUser;
		const discord::Result Result = Core->UserManager().GetCurrentUser(&CurrentUser);
		if (Result == discord::Result::Ok)
		{
			return CurrentUser.GetUsername();
		}
	}
	return "Unknown Discord User";
}

bool UDiscordHelper::UpdatePlayActivity(const FString& State, const FDiscordRichPresenceData& Data, int64 Timestamp)
{
	if (!Core)
	{
		return false;
	}

	discord::Activity Activity{};
	Activity.SetType(discord::ActivityType::Playing);
	Activity.SetState(TCHAR_TO_ANSI(*State));
	Activity.SetDetails(TCHAR_TO_ANSI(*Data.Details.ToString()));
	Activity.GetAssets().SetLargeImage(TCHAR_TO_ANSI(*Data.LargeImageKey));
	Activity.GetAssets().SetSmallImage(TCHAR_TO_ANSI(*Data.SmallImageKey));
	discord::ActivityTimestamps& Timestamps = Activity.GetTimestamps();
	Timestamps.SetStart(Timestamp);
	Core->ActivityManager().UpdateActivity(Activity, [State](discord::Result Result)
		{
			if (Result != discord::Result::Ok)
			{
				UE_LOG(LogDiscord, Log, TEXT("Discord activity change request is failed"));
			}
			else
			{
				UE_LOG(LogDiscord, Log, TEXT("Discord activity state is changed as %s"), *State);
			}
		});
	return true;
}

void UDiscordHelper::ClearPlayActivity()
{
	if (Core)
	{
		Core->ActivityManager().ClearActivity([](discord::Result Result)
			{
				if (Result != discord::Result::Ok)
				{
					UE_LOG(LogDiscord, Log, TEXT("Discord activity clear request is failed"));
				}
				else
				{
					UE_LOG(LogDiscord, Log, TEXT("Discord activity is cleared"));
				}
			});
	}
}

void UDiscordHelper::ValidateOrExit(std::function<void(discord::Result)> Callback)
{
	if (Core)
	{
		Core->ApplicationManager().ValidateOrExit(Callback);
	}
	else
	{
		Callback(discord::Result::InternalError);
	}
}

bool UDiscordHelper::RegisterSteam(uint32 SteamAppID)
{
	if (!Core)
	{
		return false;
	}

	return Core->ActivityManager().RegisterSteam(SteamAppID) == discord::Result::Ok;
}