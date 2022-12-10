// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DiscordTypes.h"
#include "DiscordUserManager.generated.h"

// fast forwards
namespace discord
{
	class UserManager;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiscordCurrentUserUpdate);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDiscordGetUserResult, bool, bResult, const FDiscordUser&, User);

/**
 * 
 */
UCLASS(Blueprintable)
class DISCORDGSDK_API UDiscordUserManager : public UObject, public IDiscordInterface
{
	GENERATED_BODY()

public:
	virtual void Create(UDiscordCore* InCore) override;

	UFUNCTION(BlueprintCallable, Category = "Discord|User")
	bool GetCurrentUser(FDiscordUser& User);
	UFUNCTION(BlueprintCallable, Category = "Discord|User")
	void GetUser(int64 UserID, FDiscordGetUserResult Callback);
	UFUNCTION(BlueprintCallable, Category = "Discord|User")
	bool GetCurrentUserPremiumType(TEnumAsByte<FDiscordPremiumType::Type>& PremiumType);
	UFUNCTION(BlueprintCallable, Category = "Discord|User")
	bool CurrentUserHasFlag(const TEnumAsByte<FDiscordUserFlag::Type>& Flag);

	UPROPERTY(BlueprintAssignable, Category = "Discord|User")
	FDiscordCurrentUserUpdate OnCurrentUserUpdate;
};