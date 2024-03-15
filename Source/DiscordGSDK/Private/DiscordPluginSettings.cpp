// Fill out your copyright notice in the Description page of Project Settings.

#include "DiscordPluginSettings.h"

UDiscordPluginSettings::UDiscordPluginSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ClientID(0)
	, SteamAppID(480)
	, LogVerbosity(FDiscordLogLevel::DDL_Info)
	, ReconnectLimit(10)
{ }