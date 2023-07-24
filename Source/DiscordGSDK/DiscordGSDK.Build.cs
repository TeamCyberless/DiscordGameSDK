// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;
using EpicGames.Core;

public class DiscordGSDK : ModuleRules
{
	public DiscordGSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;
		
		if (Directory.Exists(Path.Combine(ModuleDirectory, "ThirdParty", "Discord")))
		{
			PublicDefinitions.Add(String.Format("DISCORD_GAMESDK_DYNAMIC_LIB={0}", 1));

			PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty",  "Discord"));
		
			string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "ThirdParty", "DiscordGSDKLibrary"));

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
			PublicDefinitions.Add(String.Format("DISCORD_GAMESDK_DYNAMIC_LIB={0}", 0));
		}

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Projects"	
			}
			);
	}
}
