// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DiscordTypes.h"
#include "DiscordActivityManager.generated.h"

// fast forwards
namespace discord
{
	class ActivityManager;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiscordActivityUpdated, const bool, Updated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiscordOnActivityJoin, const FString&, JoinSecret);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiscordOnActivitySpectate, const FString&, SpectateSecret);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiscordOnActivityJoinRequest, const FDiscordUser&, RequestedUser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDiscordOnActivityInvite, const bool, bIsSpectate, const FDiscordUser&, RequestedUser, const FDiscordActivity&, Activity);

/**
 * 
 */
UCLASS(Blueprintable)
class DISCORDGSDK_API UDiscordActivityManager : public UObject, public IDiscordInterface
{
	GENERATED_BODY()

public:
	virtual void Create(UDiscordCore* InCore) override;

	UFUNCTION(BlueprintCallable, Category = "Discord|Activity")
	bool RegisterCommand(const FString& Command);
	UFUNCTION(BlueprintCallable, Category = "Discord|Activity")
	bool RegisterSteam(int32 SteamAppID);
	UFUNCTION(BlueprintCallable, Category = "Discord|Activity")
	void UpdateActivity(FDiscordActivity NewActivity);
	UFUNCTION(BlueprintCallable, Category = "Discord|Activity")
	void ClearActivity();
	UFUNCTION(BlueprintCallable, Category = "Discord|Activity")
	void SendRequestReply(int64 UserID, TEnumAsByte<FDiscordActivityJoinRequestReply::Type> Reply);
	UFUNCTION(BlueprintCallable, Category = "Discord|Activity")
	void SendInvite(int64 UserID, bool bIsSpectate, const FString& Content);
	UFUNCTION(BlueprintCallable, Category = "Discord|Activity")
	void AcceptInvite(int64 UserID);

	UPROPERTY(BlueprintAssignable, Category = "Discord|Activity")
	FDiscordActivityUpdated OnActivityUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Discord|Activity")
	FDiscordOnActivityJoin OnActivityJoin;
	UPROPERTY(BlueprintAssignable, Category = "Discord|Activity")
	FDiscordOnActivitySpectate OnActivitySpectate;
	UPROPERTY(BlueprintAssignable, Category = "Discord|Activity")
	FDiscordOnActivityJoinRequest OnActivityJoinRequest;
	UPROPERTY(BlueprintAssignable, Category = "Discord|Activity")
	FDiscordOnActivityInvite OnActivityInvite;
};
