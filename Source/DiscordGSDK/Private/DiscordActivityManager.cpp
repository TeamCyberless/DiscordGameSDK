// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordActivityManager.h"
#include "DiscordCore.h"

#if DISCORD_GAMESDK_DYNAMIC_LIB
THIRD_PARTY_INCLUDES_START
#include "Discord/core.h"
THIRD_PARTY_INCLUDES_END
#endif

#define LOG_ERROR( Result ) \
	UE_LOG(LogDiscord, Error, TEXT("%s Failed! Error Code: %d"), *FString(__FUNCTION__), static_cast<int32>(Result))

void UDiscordActivityManager::Create(UDiscordCore* InCore)
{
	IDiscordInterface::Create(InCore);

#if DISCORD_GAMESDK_DYNAMIC_LIB
	{
		discord::Event<char const*> OnActivityJoinEvent;
		auto ActivityJoinHandler = [&](char const* JoinSecret)
		{
			OnActivityJoin.Broadcast(UTF8_TO_TCHAR(JoinSecret));
		};
		OnActivityJoinEvent.Connect(ActivityJoinHandler);
		GetCore()->ActivityManager().OnActivityJoin = OnActivityJoinEvent;
	}
	
	{
		discord::Event<char const*> OnActivitySpectateEvent;
		auto ActivitySpectateHandler = [&](char const* SpectateSecret)
		{
			OnActivitySpectate.Broadcast(UTF8_TO_TCHAR(SpectateSecret));
		};
		OnActivitySpectateEvent.Connect(ActivitySpectateHandler);
		GetCore()->ActivityManager().OnActivitySpectate = OnActivitySpectateEvent;
	}

	{
		discord::Event<discord::User const&> OnActivityJoinRequestEvent;
		auto ActivityJoinRequestHandler = [&](discord::User const& User)
		{
			FDiscordUser inUser;
			inUser.UserID = User.GetId();
			inUser.Username = User.GetUsername();
			inUser.Discriminator = User.GetDiscriminator();
			inUser.Avatar = User.GetAvatar();
			
			OnActivityJoinRequest.Broadcast(inUser);
		};
		OnActivityJoinRequestEvent.Connect(ActivityJoinRequestHandler);
		GetCore()->ActivityManager().OnActivityJoinRequest = OnActivityJoinRequestEvent;
	}

	{
		discord::Event<discord::ActivityActionType,discord:: User const&, discord::Activity const&> OnActivityInviteEvent;
		auto ActivityInviteHandler = [&](discord::ActivityActionType Type, discord::User const& User, discord::Activity const& Activity)
		{
			FDiscordUser inUser;
			inUser.UserID = User.GetId();
			inUser.Username = User.GetUsername();
			inUser.Discriminator = User.GetDiscriminator();
			inUser.Avatar = User.GetAvatar();

			FDiscordActivity inActivity;
			inActivity.State = Activity.GetState();
			inActivity.Details = Activity.GetDetails();
			inActivity.StartTimestamp = Activity.GetTimestamps().GetStart();
			inActivity.EndTimestamp = Activity.GetTimestamps().GetEnd();
			inActivity.LargeImageKey = Activity.GetAssets().GetLargeImage();
			inActivity.LargeImageText = Activity.GetAssets().GetLargeText();
			inActivity.SmallImageKey = Activity.GetAssets().GetSmallImage();
			inActivity.SmallImageText = Activity.GetAssets().GetSmallText();
			inActivity.PartyID = Activity.GetParty().GetId();
			inActivity.CurrentSize = Activity.GetParty().GetSize().GetCurrentSize();
			inActivity.MaxSize = Activity.GetParty().GetSize().GetMaxSize();
			inActivity.MatchSecret = Activity.GetSecrets().GetMatch();
			inActivity.JoinSecret = Activity.GetSecrets().GetJoin();
			inActivity.SpectateSecret = Activity.GetSecrets().GetSpectate();
			inActivity.Instance = Activity.GetInstance();
			
			OnActivityInvite.Broadcast(Type == discord::ActivityActionType::Spectate, inUser, inActivity);
		};
		OnActivityInviteEvent.Connect(ActivityInviteHandler);
		GetCore()->ActivityManager().OnActivityInvite = OnActivityInviteEvent;
	}
#endif
}

bool UDiscordActivityManager::RegisterCommand(const FString& Command)
{
#if DISCORD_GAMESDK_DYNAMIC_LIB
	if (GetCore())
	{
		const discord::Result Result = GetCore()->ActivityManager().RegisterCommand(TCHAR_TO_UTF8(*Command));

		if (Result != discord::Result::Ok)
		{
			LOG_ERROR(Result);
		}
		
		return Result == discord::Result::Ok;
	}
#endif

	return false;
}

bool UDiscordActivityManager::RegisterSteam(int32 SteamAppID)
{
#if DISCORD_GAMESDK_DYNAMIC_LIB
	if (GetCore())
	{
		const discord::Result Result = GetCore()->ActivityManager().RegisterSteam(SteamAppID);

		if (Result != discord::Result::Ok)
		{
			LOG_ERROR(Result);
		}
		
		return Result == discord::Result::Ok;
	}
#endif

	return false;
}

void UDiscordActivityManager::UpdateActivity(FDiscordActivity NewActivity)
{
#if DISCORD_GAMESDK_DYNAMIC_LIB
	if (GetCore())
	{
		auto activity = discord::Activity();
		activity.SetState(TCHAR_TO_UTF8(*NewActivity.State));
		activity.SetDetails(TCHAR_TO_UTF8(*NewActivity.Details));
		activity.GetTimestamps().SetStart(NewActivity.StartTimestamp);
		activity.GetTimestamps().SetEnd(NewActivity.EndTimestamp);
		activity.GetAssets().SetLargeImage(TCHAR_TO_UTF8(*NewActivity.LargeImageKey));
		activity.GetAssets().SetLargeText(TCHAR_TO_UTF8(*NewActivity.LargeImageText));
		activity.GetAssets().SetSmallImage(TCHAR_TO_UTF8(*NewActivity.SmallImageKey));
		activity.GetAssets().SetSmallText(TCHAR_TO_UTF8(*NewActivity.SmallImageText));
		activity.GetParty().SetId(TCHAR_TO_UTF8(*NewActivity.PartyID));
		activity.GetParty().GetSize().SetCurrentSize(NewActivity.CurrentSize);
		activity.GetParty().GetSize().SetMaxSize(NewActivity.MaxSize);
		activity.GetSecrets().SetMatch(TCHAR_TO_UTF8(*NewActivity.MatchSecret));
		activity.GetSecrets().SetJoin(TCHAR_TO_UTF8(*NewActivity.JoinSecret));
		activity.GetSecrets().SetSpectate(TCHAR_TO_UTF8(*NewActivity.SpectateSecret));
		activity.SetInstance(NewActivity.Instance);

		auto UpdateActivityHandler = [&](discord::Result Result)
		{
			OnActivityUpdated.Broadcast(Result == discord::Result::Ok);
			
			if (Result != discord::Result::Ok)
			{
				LOG_ERROR(Result);
			}
		};
		
		GetCore()->ActivityManager().UpdateActivity(activity, UpdateActivityHandler);
	}
#endif
}

void UDiscordActivityManager::ClearActivity()
{
#if DISCORD_GAMESDK_DYNAMIC_LIB
	if (GetCore())
	{
		auto UpdateActivityHandler = [&](discord::Result Result)
		{
			OnActivityUpdated.Broadcast(Result == discord::Result::Ok);
			
			if (Result != discord::Result::Ok)
			{
				LOG_ERROR(Result);
			}
		};
		
		GetCore()->ActivityManager().ClearActivity(UpdateActivityHandler);
	}
#endif
}

void UDiscordActivityManager::SendRequestReply(int64 UserID, TEnumAsByte<FDiscordActivityJoinRequestReply::Type> Reply)
{
#if DISCORD_GAMESDK_DYNAMIC_LIB
	if (GetCore())
	{
		auto RequestHandler = [&](discord::Result Result)
		{			
			if (Result != discord::Result::Ok)
			{
				LOG_ERROR(Result);
			}
		};

		discord::ActivityJoinRequestReply inReply;
		switch (Reply)
		{
			default: case FDiscordActivityJoinRequestReply::Ignore: inReply = discord::ActivityJoinRequestReply::Ignore; break;
			case FDiscordActivityJoinRequestReply::Yes: inReply = discord::ActivityJoinRequestReply::Yes; break;
			case FDiscordActivityJoinRequestReply::No: inReply = discord::ActivityJoinRequestReply::No; break;
		}
		
		GetCore()->ActivityManager().SendRequestReply(UserID, inReply, RequestHandler);
	}
#endif
}

void UDiscordActivityManager::SendInvite(int64 UserID, bool bIsSpectate, const FString& Content)
{
#if DISCORD_GAMESDK_DYNAMIC_LIB
	if (GetCore())
	{
		auto RequestHandler = [&](discord::Result Result)
		{			
			if (Result != discord::Result::Ok)
			{
				LOG_ERROR(Result);
			}
		};
		
		GetCore()->ActivityManager().SendInvite(UserID, bIsSpectate ? discord::ActivityActionType::Spectate : discord::ActivityActionType::Join, TCHAR_TO_UTF8(*Content), RequestHandler);
	}
#endif
}

void UDiscordActivityManager::AcceptInvite(int64 UserID)
{
#if DISCORD_GAMESDK_DYNAMIC_LIB
	if (GetCore())
	{
		auto RequestHandler = [&](discord::Result Result)
		{			
			if (Result != discord::Result::Ok)
			{
				LOG_ERROR(Result);
			}
		};
		
		GetCore()->ActivityManager().AcceptInvite(UserID, RequestHandler);
	}
#endif
}
