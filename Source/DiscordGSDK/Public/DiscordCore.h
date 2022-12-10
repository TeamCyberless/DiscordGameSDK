// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DiscordTypes.h"
#include "Tickable.h"
#include "DiscordCore.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiscord, All, All)

// fast forwards
namespace discord
{
	class Core;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDiscordLogHook, TEnumAsByte<FDiscordLogLevel::Type>, inLogLevel, const FString&, inText);

/**
 * 
 */
UCLASS(Blueprintable)
class DISCORDGSDK_API UDiscordCore : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Discord|Core")
	TEnumAsByte<FDiscordResult::Type> Create(int64 ClientID, bool bIsDiscordRequired);

	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Core")
	FDiscordLogHook OnDiscordLogHook;

	// @TODO: Discord Interfaces
	
private:
	discord::Core* Core = nullptr;
};

