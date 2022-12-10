// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordUserManager.h"
#include "DiscordCore.h"
#include "ThirdParty/DiscordGSDKLibrary/Include/core.h"

#define LOG_ERROR( Result ) \
	UE_LOG(LogDiscord, Error, TEXT("%s Failed! Error Code: %d"), *FString(__FUNCTION__), static_cast<int32>(Result))

void UDiscordUserManager::Create(UDiscordCore* InCore)
{
	IDiscordInterface::Create(InCore);
	
	discord::Event<> OnUserConnectedEvent;
	auto UserConnectionHandler = [&]()
	{
		OnCurrentUserUpdate.Broadcast();
	};
	OnUserConnectedEvent.Connect(UserConnectionHandler);
	GetCore()->UserManager().OnCurrentUserUpdate = OnUserConnectedEvent;
}

bool UDiscordUserManager::GetCurrentUser(FDiscordUser& User)
{
	if (GetCore())
	{
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
	}

	return false;
}

void UDiscordUserManager::GetUser(int64 UserID, FDiscordGetUserResult Callback)
{
	if (GetCore())
	{
		auto GetUserCallback = [&](discord::Result Result, discord::User User)
		{
			if (Result == discord::Result::Ok)
			{
				FDiscordUser inUser;
				inUser.UserID = User.GetId();
				inUser.Username = User.GetUsername();
				inUser.Discriminator = User.GetDiscriminator();
				inUser.Avatar = User.GetAvatar();

				Callback.ExecuteIfBound(true, inUser);
			}
			else
			{
				LOG_ERROR(Result);
				Callback.ExecuteIfBound(false, FDiscordUser());
			}
		};
		
		GetCore()->UserManager().GetUser(UserID, GetUserCallback);
	}
}

bool UDiscordUserManager::GetCurrentUserPremiumType(
	TEnumAsByte<FDiscordPremiumType::Type>& PremiumType)
{
	if (GetCore())
	{
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
	}

	return false;
}
