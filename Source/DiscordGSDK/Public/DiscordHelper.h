// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "types.h"
#include "Tickable.h"
#include "Engine/DataTable.h"
#include "DiscordHelper.generated.h"

namespace discord
{
	class Core;
}

USTRUCT(BlueprintType)
struct FDiscordRichPresenceData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Discord)
	FText Details;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Discord)
	FString LargeImageKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Discord)
	FString SmallImageKey;
};

/**
 * 
 */
UCLASS()
class DISCORDGSDK_API UDiscordHelper : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	bool Initialize(int64 ClientID, bool bDiscordRequired = false);

	virtual void BeginDestroy() override;

	// Tickable object interface
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	int64 GetConnectedUserID();
	FString GetConnectedUserName();

	bool UpdatePlayActivity(const FString& State, const FDiscordRichPresenceData& Data, int64 Timestamp);
	void ClearPlayActivity();

	void ValidateOrExit(std::function<void(discord::Result)> Callback);

	bool RegisterSteam(uint32 SteamAppID);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUserConnectedSignature, const FString&, UserName, const int64, UserId, const FString&, UserDiscriminator);
	UPROPERTY(BlueprintAssignable)
	FOnUserConnectedSignature OnUserConnected;

private:
	discord::Core* Core = nullptr;
	bool bGotUserConnectedReply = false;
};
