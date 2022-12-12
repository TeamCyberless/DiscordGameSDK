// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordGSDK.h"
#include "HAL/PlatformProcess.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "DiscordPluginSettings.h"

#define LOCTEXT_NAMESPACE "FDiscordGSDKModule"

void FDiscordGSDKModule::StartupModule()
{
#if !PLATFORM_LINUX
#if defined(DISCORD_GAMESDK_DYNAMIC_LIB)
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("DiscordGSDK")->GetBaseDir();
	const FString SDKDir = FPaths::Combine(*BaseDir, TEXT("Source"), TEXT("ThirdParty"), TEXT("DiscordGSDKLibrary"));
#if PLATFORM_WINDOWS
	const FString LibName = TEXT("discord_game_sdk");
	const FString LibDir = FPaths::Combine(*SDKDir, TEXT("Win64"));
	if (!LoadDependency(LibDir, LibName, DiscordGSDKLibraryHandle))
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT(LOCTEXT_NAMESPACE, "Failed to load DiscordGameSDK plugin, Plugin will not be functional."));
		FreeDependency(DiscordGSDKLibraryHandle);
	}
#elif PLATFORM_MAC
	const FString LibName = TEXT("discord_game_sdk");
	const FString LibDir = FPaths::Combine(*SDKDir, TEXT("Mac"));
	if (!LoadDependency(LibDir, LibName, DiscordGSDKLibraryHandle))
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT(LOCTEXT_NAMESPACE, "Failed to load DiscordGameSDK plugin, Plugin will not be functional."));
		FreeDependency(DiscordGSDKLibraryHandle);
	}
#endif
#endif
#endif

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory("DiscordGameSDK",
			LOCTEXT("RuntimeWDCategoryName", "DiscordGameSDK"),
			LOCTEXT("RuntimeWDCategoryDescription", "Configuration for the DiscordGameSDK plugin"));

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "DiscordGameSDK", "General",
			LOCTEXT("RuntimeGeneralSettingsName", "General"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Base configuration for DiscordGameSDK plugin"),
			GetMutableDefault<UDiscordPluginSettings>()
			);

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FDiscordGSDKModule::HandleSettingsSaved);
		}
	}
}

void FDiscordGSDKModule::ShutdownModule()
{
	// Free the dll handle
#if !PLATFORM_LINUX
#if defined(DISCORD_GAMESDK_DYNAMIC_LIB)
	FreeDependency(DiscordGSDKLibraryHandle);
#endif
#endif

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "DiscordGameSDK", "General");
	}
}

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

bool FDiscordGSDKModule::HandleSettingsSaved()
{
	UDiscordPluginSettings* Settings = GetMutableDefault<UDiscordPluginSettings>();
	Settings->SaveConfig();

	return true;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDiscordGSDKModule, DiscordGSDK)