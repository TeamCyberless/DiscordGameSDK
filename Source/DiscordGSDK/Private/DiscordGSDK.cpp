// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordGSDK.h"
#include "DiscordPluginSettings.h"
#include "HAL/PlatformProcess.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/CoreDelegates.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FDiscordGSDKModule"

void FDiscordGSDKModule::StartupModule()
{
#if !PLATFORM_LINUX && defined(DISCORD_GAMESDK_DYNAMIC_LIB)
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("DiscordGSDK")->GetBaseDir();
	const FString SDKDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"));
#if PLATFORM_WINDOWS
	const FString LibName = TEXT("discord_game_sdk");
	const FString LibDir = FPaths::Combine(*SDKDir, TEXT("Win64"));
	if (!LoadDependency(LibDir, LibName, DiscordGSDKLibraryHandle))
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT(LOCTEXT_NAMESPACE, "Failed to load DiscordGSDK plugin, Plugin will not be functional."));
		FreeDependency(DiscordGSDKLibraryHandle);
	}
#elif PLATFORM_MAC
	const FString LibName = TEXT("discord_game_sdk");
	const FString LibDir = FPaths::Combine(*SDKDir, TEXT("Mac"));
	if (!LoadDependency(LibDir, LibName, DiscordGSDKLibraryHandle))
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT(LOCTEXT_NAMESPACE, "Failed to load DiscordGSDK plugin, Plugin will not be functional."));
		FreeDependency(DiscordGSDKLibraryHandle);
	}
#endif
#endif

#if WITH_EDITOR
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FDiscordGSDKModule::OnPostEngineInit);
	FCoreDelegates::OnPreExit.AddRaw(this, &FDiscordGSDKModule::OnPreExit);
#endif
}

void FDiscordGSDKModule::ShutdownModule()
{
	// Free the dll handle
#if !PLATFORM_LINUX && defined(DISCORD_GAMESDK_DYNAMIC_LIB)
	FreeDependency(DiscordGSDKLibraryHandle);
#endif
}

#if WITH_EDITOR
void FDiscordGSDKModule::OnPostEngineInit()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Game", "Discord Game SDK",
			LOCTEXT("DiscordGameSDKSettingsName", "Discord Game SDK"),
			LOCTEXT("DiscordGameSDKSettingsDescription", "Discord Game SDK Settings"),
			GetMutableDefault<UDiscordPluginSettings>());
	}
}

void FDiscordGSDKModule::OnPreExit()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Game", "Discord Game SDK");
	}
}
#endif

bool FDiscordGSDKModule::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
{
	FString Lib = Name + TEXT(".") + FPlatformProcess::GetModuleExtension();
	FString Path = Dir.IsEmpty() ? *Lib : FPaths::Combine(*Dir, *Lib);

	Handle = FPlatformProcess::GetDllHandle(*Path);

	if (Handle == nullptr) return false;

	return true;
}

void FDiscordGSDKModule::FreeDependency(void*& Handle)
{
	if (Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDiscordGSDKModule, DiscordGSDK)