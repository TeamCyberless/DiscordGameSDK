// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordRelationshipManager.h"
#include "DiscordCore.h"
#if DISCORD_GAMESDK_DYNAMIC_LIB
#include "DiscordGSDK/ThirdParty/Discord/core.h"
#endif

#define LOG_ERROR( Result ) \
UE_LOG(LogDiscord, Error, TEXT("%s Failed! Error Code: %d"), *FString(__FUNCTION__), static_cast<int32>(Result))

void UDiscordRelationshipManager::Create(UDiscordCore* InCore)
{
	IDiscordInterface::Create(InCore);

#if DISCORD_GAMESDK_DYNAMIC_LIB
	{
		discord::Event<> OnRelationshipRefreshEvent;
		auto RelationshipRefreshHandler = [&]()
		{
			OnRelationshipRefresh.Broadcast();
		};
		OnRelationshipRefreshEvent.Connect(RelationshipRefreshHandler);
		GetCore()->RelationshipManager().OnRefresh = OnRelationshipRefreshEvent;
	}
	
	{
    	discord::Event<discord::Relationship const&> OnRelationshipUpdateEvent;
    	auto RelationshipUpdateHandler = [&](discord::Relationship const& Relationship)
    	{
			FDiscordRelationship inRelationship;
    		inRelationship.Activity.State = Relationship.GetPresence().GetActivity().GetState();
    		inRelationship.Activity.Details = Relationship.GetPresence().GetActivity().GetDetails();
    		inRelationship.Activity.StartTimestamp = Relationship.GetPresence().GetActivity().GetTimestamps().GetStart();
    		inRelationship.Activity.EndTimestamp = Relationship.GetPresence().GetActivity().GetTimestamps().GetEnd();
    		inRelationship.Activity.LargeImageKey = Relationship.GetPresence().GetActivity().GetAssets().GetLargeImage();
    		inRelationship.Activity.LargeImageText = Relationship.GetPresence().GetActivity().GetAssets().GetLargeText();
    		inRelationship.Activity.SmallImageKey = Relationship.GetPresence().GetActivity().GetAssets().GetSmallImage();
    		inRelationship.Activity.SmallImageText = Relationship.GetPresence().GetActivity().GetAssets().GetSmallText();
    		inRelationship.Activity.PartyID = Relationship.GetPresence().GetActivity().GetParty().GetId();
    		inRelationship.Activity.CurrentSize = Relationship.GetPresence().GetActivity().GetParty().GetSize().GetCurrentSize();
    		inRelationship.Activity.MaxSize = Relationship.GetPresence().GetActivity().GetParty().GetSize().GetMaxSize();
    		inRelationship.Activity.MatchSecret = Relationship.GetPresence().GetActivity().GetSecrets().GetMatch();
    		inRelationship.Activity.JoinSecret = Relationship.GetPresence().GetActivity().GetSecrets().GetJoin();
    		inRelationship.Activity.SpectateSecret = Relationship.GetPresence().GetActivity().GetSecrets().GetSpectate();
    		inRelationship.Activity.Instance = Relationship.GetPresence().GetActivity().GetInstance();

    		inRelationship.Status = static_cast<FDiscordStatus::Type>(Relationship.GetPresence().GetStatus());

    		inRelationship.User.UserID = Relationship.GetUser().GetId();
    		inRelationship.User.Username = Relationship.GetUser().GetUsername();
    		inRelationship.User.Discriminator = Relationship.GetUser().GetDiscriminator();
    		inRelationship.User.Avatar = Relationship.GetUser().GetAvatar();

    		inRelationship.RelationshipType = static_cast<FDiscordRelationshipType::Type>(Relationship.GetType());
    		
    		OnRelationshipUpdate.Broadcast(inRelationship);
    	};
    	OnRelationshipUpdateEvent.Connect(RelationshipUpdateHandler);
    	GetCore()->RelationshipManager().OnRelationshipUpdate = OnRelationshipUpdateEvent;
	}
#endif
}

void UDiscordRelationshipManager::Filter(FDiscordRelationshipFilter FilterCallback)
{
	if (Core)
	{
#if DISCORD_GAMESDK_DYNAMIC_LIB
		auto RelationshipFilter = [&](discord::Relationship const& Relationship) -> bool
		{
			if (!FilterCallback.IsBound())
			{
				return true;
			}
			
			FDiscordRelationship inRelationship;
			inRelationship.Activity.State = Relationship.GetPresence().GetActivity().GetState();
			inRelationship.Activity.Details = Relationship.GetPresence().GetActivity().GetDetails();
			inRelationship.Activity.StartTimestamp = Relationship.GetPresence().GetActivity().GetTimestamps().GetStart();
            inRelationship.Activity.EndTimestamp = Relationship.GetPresence().GetActivity().GetTimestamps().GetEnd();
			inRelationship.Activity.LargeImageKey = Relationship.GetPresence().GetActivity().GetAssets().GetLargeImage();
            inRelationship.Activity.LargeImageText = Relationship.GetPresence().GetActivity().GetAssets().GetLargeText();
            inRelationship.Activity.SmallImageKey = Relationship.GetPresence().GetActivity().GetAssets().GetSmallImage();
            inRelationship.Activity.SmallImageText = Relationship.GetPresence().GetActivity().GetAssets().GetSmallText();
            inRelationship.Activity.PartyID = Relationship.GetPresence().GetActivity().GetParty().GetId();
            inRelationship.Activity.CurrentSize = Relationship.GetPresence().GetActivity().GetParty().GetSize().GetCurrentSize();
            inRelationship.Activity.MaxSize = Relationship.GetPresence().GetActivity().GetParty().GetSize().GetMaxSize();
            inRelationship.Activity.MatchSecret = Relationship.GetPresence().GetActivity().GetSecrets().GetMatch();
            inRelationship.Activity.JoinSecret = Relationship.GetPresence().GetActivity().GetSecrets().GetJoin();
            inRelationship.Activity.SpectateSecret = Relationship.GetPresence().GetActivity().GetSecrets().GetSpectate();
            inRelationship.Activity.Instance = Relationship.GetPresence().GetActivity().GetInstance();
            
            inRelationship.Status = static_cast<FDiscordStatus::Type>(Relationship.GetPresence().GetStatus());
            
            inRelationship.User.UserID = Relationship.GetUser().GetId();
            inRelationship.User.Username = Relationship.GetUser().GetUsername();
            inRelationship.User.Discriminator = Relationship.GetUser().GetDiscriminator();
            inRelationship.User.Avatar = Relationship.GetUser().GetAvatar();
            
            inRelationship.RelationshipType = static_cast<FDiscordRelationshipType::Type>(Relationship.GetType());
            
			return FilterCallback.Execute(inRelationship);
		};
		
		GetCore()->RelationshipManager().Filter(RelationshipFilter);
#endif
	}
}

bool UDiscordRelationshipManager::Count(int32& Count)
{
	if (Core)
	{
#if DISCORD_GAMESDK_DYNAMIC_LIB
		int32_t CountT;
		const discord::Result Result = GetCore()->RelationshipManager().Count(&CountT);
		Count = CountT;

		if (Result != discord::Result::Ok)
		{
			LOG_ERROR(Result);
		}

		return Result == discord::Result::Ok;
#endif
	}

	return false;
}

bool UDiscordRelationshipManager::Get(int64 UserID, FDiscordRelationship& Relationship)
{
	if (Core)
	{
#if DISCORD_GAMESDK_DYNAMIC_LIB
		discord::Relationship inRelationship;
		const discord::Result Result = GetCore()->RelationshipManager().Get(UserID, &inRelationship);

		if (Result != discord::Result::Ok)
		{
			LOG_ERROR(Result);
		}
		else
		{
			Relationship.Activity.State = inRelationship.GetPresence().GetActivity().GetState();
			Relationship.Activity.Details = inRelationship.GetPresence().GetActivity().GetDetails();
			Relationship.Activity.StartTimestamp = inRelationship.GetPresence().GetActivity().GetTimestamps().GetStart();
            Relationship.Activity.EndTimestamp = inRelationship.GetPresence().GetActivity().GetTimestamps().GetEnd();
			Relationship.Activity.LargeImageKey = inRelationship.GetPresence().GetActivity().GetAssets().GetLargeImage();
            Relationship.Activity.LargeImageText = inRelationship.GetPresence().GetActivity().GetAssets().GetLargeText();
            Relationship.Activity.SmallImageKey = inRelationship.GetPresence().GetActivity().GetAssets().GetSmallImage();
            Relationship.Activity.SmallImageText = inRelationship.GetPresence().GetActivity().GetAssets().GetSmallText();
            Relationship.Activity.PartyID = inRelationship.GetPresence().GetActivity().GetParty().GetId();
            Relationship.Activity.CurrentSize = inRelationship.GetPresence().GetActivity().GetParty().GetSize().GetCurrentSize();
            Relationship.Activity.MaxSize = inRelationship.GetPresence().GetActivity().GetParty().GetSize().GetMaxSize();
            Relationship.Activity.MatchSecret = inRelationship.GetPresence().GetActivity().GetSecrets().GetMatch();
            Relationship.Activity.JoinSecret = inRelationship.GetPresence().GetActivity().GetSecrets().GetJoin();
            Relationship.Activity.SpectateSecret = inRelationship.GetPresence().GetActivity().GetSecrets().GetSpectate();
            Relationship.Activity.Instance = inRelationship.GetPresence().GetActivity().GetInstance();
            
            Relationship.Status = static_cast<FDiscordStatus::Type>(inRelationship.GetPresence().GetStatus());
            
            Relationship.User.UserID = inRelationship.GetUser().GetId();
            Relationship.User.Username = inRelationship.GetUser().GetUsername();
            Relationship.User.Discriminator = inRelationship.GetUser().GetDiscriminator();
            Relationship.User.Avatar = inRelationship.GetUser().GetAvatar();
            
            Relationship.RelationshipType = static_cast<FDiscordRelationshipType::Type>(inRelationship.GetType());
		}

		return Result == discord::Result::Ok;
#endif
	}

	return false;
}

bool UDiscordRelationshipManager::GetAt(int32 Index, FDiscordRelationship& Relationship)
{
	if (Core)
	{
#if DISCORD_GAMESDK_DYNAMIC_LIB
		discord::Relationship inRelationship;
		const discord::Result Result = GetCore()->RelationshipManager().GetAt(Index, &inRelationship);

		if (Result != discord::Result::Ok)
		{
			LOG_ERROR(Result);
		}
		else
		{
			Relationship.Activity.State = inRelationship.GetPresence().GetActivity().GetState();
			Relationship.Activity.Details = inRelationship.GetPresence().GetActivity().GetDetails();
			Relationship.Activity.StartTimestamp = inRelationship.GetPresence().GetActivity().GetTimestamps().GetStart();
            Relationship.Activity.EndTimestamp = inRelationship.GetPresence().GetActivity().GetTimestamps().GetEnd();
			Relationship.Activity.LargeImageKey = inRelationship.GetPresence().GetActivity().GetAssets().GetLargeImage();
            Relationship.Activity.LargeImageText = inRelationship.GetPresence().GetActivity().GetAssets().GetLargeText();
            Relationship.Activity.SmallImageKey = inRelationship.GetPresence().GetActivity().GetAssets().GetSmallImage();
            Relationship.Activity.SmallImageText = inRelationship.GetPresence().GetActivity().GetAssets().GetSmallText();
            Relationship.Activity.PartyID = inRelationship.GetPresence().GetActivity().GetParty().GetId();
            Relationship.Activity.CurrentSize = inRelationship.GetPresence().GetActivity().GetParty().GetSize().GetCurrentSize();
            Relationship.Activity.MaxSize = inRelationship.GetPresence().GetActivity().GetParty().GetSize().GetMaxSize();
            Relationship.Activity.MatchSecret = inRelationship.GetPresence().GetActivity().GetSecrets().GetMatch();
            Relationship.Activity.JoinSecret = inRelationship.GetPresence().GetActivity().GetSecrets().GetJoin();
            Relationship.Activity.SpectateSecret = inRelationship.GetPresence().GetActivity().GetSecrets().GetSpectate();
            Relationship.Activity.Instance = inRelationship.GetPresence().GetActivity().GetInstance();
            
            Relationship.Status = static_cast<FDiscordStatus::Type>(inRelationship.GetPresence().GetStatus());
            
            Relationship.User.UserID = inRelationship.GetUser().GetId();
            Relationship.User.Username = inRelationship.GetUser().GetUsername();
            Relationship.User.Discriminator = inRelationship.GetUser().GetDiscriminator();
            Relationship.User.Avatar = inRelationship.GetUser().GetAvatar();
            
            Relationship.RelationshipType = static_cast<FDiscordRelationshipType::Type>(inRelationship.GetType());
		}

		return Result == discord::Result::Ok;
#endif
	}

	return false;
}
