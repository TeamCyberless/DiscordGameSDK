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
		PublicDefinitions.Add(String.Format("DISCORD_GAMESDK_DYNAMIC_LIB={0}", 1));

		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DiscordGSDKLibrary"
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
