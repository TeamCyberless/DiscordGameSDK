// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using Microsoft.Extensions.Logging;
using UnrealBuildTool;

public class DiscordGSDK : ModuleRules
{
	public DiscordGSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IWYUSupport = IWYUSupport.Full;
		
		if (Directory.Exists(Path.Combine(PluginDirectory, "ThirdParty")))
		{
			Logger.LogInformation("DiscordGSDK Initalized.");
			PublicDefinitions.Add("DISCORD_GAMESDK_DYNAMIC_LIB=1");

			PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		
			string BaseDirectory = Path.GetFullPath(Path.Combine(PluginDirectory, "ThirdParty"));

			if (Target.Platform == UnrealTargetPlatform.Win64)
			{
				PublicAdditionalLibraries.Add(Path.Combine(BaseDirectory, "Win64", "discord_game_sdk.dll.lib"));
				RuntimeDependencies.Add(Path.Combine(BaseDirectory, "Win64", "discord_game_sdk.dll"));
				PublicDelayLoadDLLs.Add("discord_game_sdk.dll");
			}
			else if (Target.Platform == UnrealTargetPlatform.Linux)
			{
				PublicAdditionalLibraries.Add(Path.Combine(BaseDirectory, "Linux", "x86_64-unknown-linux-gnu", "discord_game_sdk.so") );
				RuntimeDependencies.Add(Path.Combine(BaseDirectory, "Linux", "x86_64-unknown-linux-gnu", "discord_game_sdk.so"));
			}
			else if (Target.Platform == UnrealTargetPlatform.Mac)
			{
				PublicAdditionalLibraries.Add(Path.Combine(BaseDirectory, "Mac", "discord_game_sdk.dylib") );
				RuntimeDependencies.Add(Path.Combine(BaseDirectory, "Mac", "discord_game_sdk.dylib"));
			}
		}
		else
		{
			Logger.LogError("There is no DiscordGSDK files ({0}).", Path.Combine(PluginDirectory, "ThirdParty"));
			PublicDefinitions.Add("DISCORD_GAMESDK_DYNAMIC_LIB=0");
		}

		PublicDependencyModuleNames.AddRange(
			[
				"Core", 
				"CoreUObject",
				"Engine"
			]
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			[
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Projects"
			]
		);
	}
}
