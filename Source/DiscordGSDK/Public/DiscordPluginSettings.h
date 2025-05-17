// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiscordTypes.h"
#include "Engine/RuntimeOptionsBase.h"
#include "DiscordPluginSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Discord Game SDK Settings"))
class DISCORDGSDK_API UDiscordPluginSettings : public URuntimeOptionsBase
{
	GENERATED_BODY()

public:	
	/** This can be obtained from Discord Developer Portal. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	int64 ClientID = 0;

	/** If this game will distributed over Steam and you want to connect with Steam. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	int32 SteamAppID = 480;

	/** Log verbosity. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	TEnumAsByte<FDiscordLogLevel::Type> LogVerbosity = FDiscordLogLevel::DDL_Info;
	
	/** Try to reconnect with Discord if we didn't exceeded this limit. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	int32 ReconnectLimit = 10;
};
