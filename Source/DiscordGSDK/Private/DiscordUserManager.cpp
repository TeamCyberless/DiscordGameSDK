// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordUserManager.h"
#include "DiscordCore.h"
#if DISCORD_GAMESDK_DYNAMIC_LIB
THIRD_PARTY_INCLUDES_START
#include "Discord/core.h"
THIRD_PARTY_INCLUDES_END
#endif

#define LOG_ERROR( Result ) \
	UE_LOG(LogDiscord, Error, TEXT("%s Failed! Error Code: %d"), *FString(__FUNCTION__), static_cast<int32>(Result))

void UDiscordUserManager::Create(UDiscordCore* InCore)
{
	IDiscordInterface::Create(InCore);

#if DISCORD_GAMESDK_DYNAMIC_LIB
	discord::Event<> OnUserConnectedEvent;
	auto UserConnectionHandler = [&]()
	{
		OnCurrentUserUpdate.Broadcast();
	};
	OnUserConnectedEvent.Connect(UserConnectionHandler);
	GetCore()->UserManager().OnCurrentUserUpdate = OnUserConnectedEvent;
#endif
}

bool UDiscordUserManager::GetCurrentUser(FDiscordUser& User)
{
	if (GetCore())
	{
#if DISCORD_GAMESDK_DYNAMIC_LIB
		discord::User* inUser = nullptr;
		const discord::Result Result = GetCore()->UserManager().GetCurrentUser(inUser);
		if (Result == discord::Result::Ok)
		{
			User.UserID = inUser->GetId();
			User.Username = inUser->GetUsername();
			User.Discriminator = inUser->GetDiscriminator();
			User.Avatar = inUser->GetAvatar();
		}
		else
		{
			LOG_ERROR(Result);
		}

		return Result == discord::Result::Ok;
#endif
	}

	return false;
}

void UDiscordUserManager::GetUser(int64 UserID)
{
	if (GetCore())
	{
#if DISCORD_GAMESDK_DYNAMIC_LIB
		auto GetUserCallback = [&](discord::Result Result, discord::User User)
		{
			if (Result == discord::Result::Ok)
			{
				FDiscordUser inUser;
				inUser.UserID = User.GetId();
				inUser.Username = User.GetUsername();
				inUser.Discriminator = User.GetDiscriminator();
				inUser.Avatar = User.GetAvatar();

				OnGetUserResult.Broadcast(true, inUser);
			}
			else
			{
				LOG_ERROR(Result);
				OnGetUserResult.Broadcast(false, FDiscordUser());
			}
		};
		
		GetCore()->UserManager().GetUser(UserID, GetUserCallback);
#endif
	}
}

bool UDiscordUserManager::GetCurrentUserPremiumType(
	TEnumAsByte<FDiscordPremiumType::Type>& PremiumType)
{
	if (GetCore())
	{
#if DISCORD_GAMESDK_DYNAMIC_LIB
		discord::PremiumType inPremiumType;
		const discord::Result Result = GetCore()->UserManager().GetCurrentUserPremiumType(&inPremiumType);
		if (Result == discord::Result::Ok)
		{
			PremiumType = static_cast<FDiscordPremiumType::Type>(inPremiumType);
			return true;
		}
		else
		{
			LOG_ERROR(Result);
		}
#endif
	}

	PremiumType = FDiscordPremiumType::None;
	return false;
}

bool UDiscordUserManager::CurrentUserHasFlag(const TEnumAsByte<FDiscordUserFlag::Type>& Flag)
{
	if (Flag == FDiscordUserFlag::None)
	{
		// We don't allow none flag.
		return false;
	}
	
	if (GetCore())
	{
#if DISCORD_GAMESDK_DYNAMIC_LIB
		bool Returnable;
		const discord::Result Result = GetCore()->UserManager().CurrentUserHasFlag(static_cast<discord::UserFlag>(Flag.GetValue()), &Returnable);
		if (Result == discord::Result::Ok)
		{
			return Returnable;
		}
		else
		{
			LOG_ERROR(Result);
		}
#endif
	}

	return false;
}
