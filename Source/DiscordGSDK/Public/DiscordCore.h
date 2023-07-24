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

class UDiscordUserManager;
class UDiscordActivityManager;
class UDiscordRelationshipManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDiscordLogHook, TEnumAsByte<FDiscordLogLevel::Type>, inLogLevel, const FString&, inText);

/**
 * 
 */
UCLASS(Blueprintable)
class DISCORDGSDK_API UDiscordCore : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	virtual void Initialize(bool bIsDiscordRequired);
	
	virtual void BeginDestroy() override;

	// Begin Tick overrides
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	// End Tick overrides

	// Begin mutators
	discord::Core* GetCore() const { return Core; }

	UFUNCTION(BlueprintCallable, Category = "Discord|Core|Interfaces")
	UDiscordUserManager* GetDiscordUserManager() const { return DiscordUserManager; }
	UFUNCTION(BlueprintCallable, Category = "Discord|Core|Interfaces")
	UDiscordActivityManager* GetDiscordActivityManager() const { return DiscordActivityManager; }
	UFUNCTION(BlueprintCallable, Category = "Discord|Core|Interfaces")
	UDiscordRelationshipManager* GetDiscordRelationshipManager() const { return DiscordRelationshipManager; } 
	// End mutators
	
	UPROPERTY(BlueprintAssignable, Category = "Discord|Core")
	FDiscordLogHook OnDiscordLogHook;

private: // Variables
	// @TODO: Discord Interfaces
	UPROPERTY()
	UDiscordUserManager* DiscordUserManager = nullptr;
	UPROPERTY()
	UDiscordActivityManager* DiscordActivityManager = nullptr;
	UPROPERTY()
	UDiscordRelationshipManager* DiscordRelationshipManager = nullptr;
	
	discord::Core* Core = nullptr;

	int ReconnectCount = 0;

private: // Private functions
	void InitializeInterfaces();
};

