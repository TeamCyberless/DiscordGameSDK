#pragma once

#include "CoreMinimal.h"
#include "types.h"
#include "DiscordTypes.generated.h"

namespace discord
{
	class Core;
}

UENUM(BlueprintType)
namespace FDiscordLogLevel
{
	enum Type
	{
		DDL_None = 0,
		DDL_Error = 1,
		DDL_Warn,
		DDL_Info,
		DDL_Debug
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

USTRUCT(BlueprintType)
struct FDiscordUser
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") int64 UserID;
	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") FString Username;
	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") FString Discriminator;
	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") FString Avatar;
	UPROPERTY(BlueprintReadOnly, Category = "Discord|User") bool bIsBot;
};

class DISCORDGSDK_API IDiscordInterface : IInterface
{
public:
	virtual void Create(class UDiscordCore* InCore)
	{
		Core = InCore;
	}

	virtual discord::Core* GetCore() const;

protected:
	UDiscordCore* Core = nullptr;
};