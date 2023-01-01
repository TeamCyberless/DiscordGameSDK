// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DiscordTypes.h"
#include "DiscordPluginSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Discord Game SDK Settings"))
class DISCORDGSDK_API UDiscordPluginSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDiscordPluginSettings(const FObjectInitializer& ObjectInitializer);

	// UDeveloperSettings overrides
	virtual FName GetContainerName() const { return FName("Editor"); }
	virtual FName GetCategoryName() const { return FName("Plugins"); }
	virtual FName GetSectionName() const { return FName("DiscordGameSDK"); }

	virtual FText GetSectionText() const;
	virtual FText GetSectionDescription() const;
	
	/** This can be obtained from Discord Developer Portal. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	int64 ClientID;

	/** If this game will distributed over Steam and you want to connect with Steam. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	int32 SteamAppID;

	/** Log verbosity. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	TEnumAsByte<FDiscordLogLevel::Type> LogVerbosity;
	
	/** Try to reconnect with Discord if we didn't exceeded this limit. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	int32 ReconnectLimit;
};
