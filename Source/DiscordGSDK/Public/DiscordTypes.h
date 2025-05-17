// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DiscordTypes.generated.h"

namespace discord
{
	class Core;
}

UENUM(BlueprintType)
enum class DISCORD_OUT_EXEC : uint8
{
	OnSuccess,
	OnFail
};

UENUM(BlueprintType)
namespace FDiscordLogLevel
{
	enum Type
	{
		DDL_None = 0 UMETA(DisplayName = "None"),
		DDL_Error = 1 UMETA(DisplayName = "Error"),
		DDL_Warn UMETA(DisplayName = "Warning"),
		DDL_Info UMETA(DisplayName = "Info"),
		DDL_Debug UMETA(DisplayName = "Debug")
	};
}

UENUM(BlueprintType)
namespace FDiscordPremiumType
{
	enum Type {
    	None = 0,
    	Tier1 = 1,
    	Tier2 = 2,
    };
}

UENUM(BlueprintType)
namespace FDiscordUserFlag
{
	enum Type {
		None = 0,
		Partner = 2,
		HypeSquadEvents = 4,
		HypeSquadHouse1 = 64,
		HypeSquadHouse2 = 128,
		HypeSquadHouse3 = 256,
	};
}

UENUM(BlueprintType)
namespace FDiscordActivityJoinRequestReply
{
	enum Type {
		No,
		Yes,
		Ignore
	};
}

UENUM(BlueprintType)
namespace FDiscordRelationshipType
{
	enum Type {
		None,
		Friend,
		Blocked,
		PendingIncoming,
		PendingOutgoing,
		Implicit,
	};
}

UENUM(BlueprintType)
namespace FDiscordStatus
{
	enum Type {
		Offline = 0,
		Online = 1,
		Idle = 2,
		DoNotDisturb = 3,
	};
}

USTRUCT(BlueprintType)
struct FDiscordUser
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") int64 UserID = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") FString Username;
	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") FString Discriminator;
	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") FString Avatar;
	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") bool bIsBot = false;
};

USTRUCT(BlueprintType)
struct FDiscordActivity
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString State; 
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString Details; 
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") int64 StartTimestamp = 0; 
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") int64 EndTimestamp = 0; 
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString LargeImageKey; 
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString LargeImageText; 
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString SmallImageKey; 
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString SmallImageText;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString PartyID;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") int32 CurrentSize = 1;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") int32 MaxSize = 1;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString MatchSecret;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString JoinSecret;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") FString SpectateSecret;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Activity") bool Instance = false;
};

USTRUCT(BlueprintType)
struct FDiscordRelationship
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Discord|Relationship") TEnumAsByte<FDiscordRelationshipType::Type> RelationshipType = FDiscordRelationshipType::None;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Relationship") FDiscordUser User;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Relationship") TEnumAsByte<FDiscordStatus::Type> Status = FDiscordStatus::Offline;
	UPROPERTY(BlueprintReadWrite, Category = "Discord|Relationship") FDiscordActivity Activity;
};

class DISCORDGSDK_API IDiscordInterface : IInterface
{
public:
	virtual void Create(class UDiscordCore* InCore)
	{
		Core = InCore;
	}

	virtual bool IsValid() const
	{
		return Core != nullptr;
	}

	virtual discord::Core* GetCore() const;

protected:
	UDiscordCore* Core = nullptr;
};