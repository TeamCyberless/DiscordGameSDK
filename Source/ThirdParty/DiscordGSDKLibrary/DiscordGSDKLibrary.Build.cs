// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;
using EpicGames.Core;

public class DiscordGSDKLibrary : ModuleRules
{
	public DiscordGSDKLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicDefinitions.Add(String.Format("DISCORD_GAMESDK_DYNAMIC_LIB={0}", 1));

		string baseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "DiscordGSDKLibrary"));

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string library = Path.Combine(baseDirectory, "Win64");
			
			// Include headers
			PublicIncludePaths.Add(Path.Combine(baseDirectory, "Include"));
			
			// Import static library
			LinkStaticLibrary(Path.Combine(library, "discord_game_sdk.dll.lib"));
			
			// Import dynamic library
			AddDllDependency(Path.Combine(library, "discord_game_sdk.dll"));
			PublicDelayLoadDLLs.Add("discord_game_sdk.dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			string library = Path.Combine(baseDirectory, "Linux", "x86_64-unknown-linux-gnu");
			
			// Include headers
			PublicIncludePaths.Add(Path.Combine(baseDirectory, "Include"));
			
			// Import library
			LinkStaticLibrary(Path.Combine(library, "libdiscord_game_sdk.so"));
			AddDllDependency(Path.Combine(library, "libdiscord_game_sdk.so"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			string library = Path.Combine(baseDirectory, "Mac");
			
			// Include headers
			PublicIncludePaths.Add(Path.Combine(baseDirectory, "Include"));
			
			// Import library
			LinkStaticLibrary(Path.Combine(library, "discord_game_sdk.dylib") );
			AddDllDependency(Path.Combine(library, "discord_game_sdk.dylib"));
		}
	}

	private void LinkStaticLibrary(string Path)
	{
		if (!File.Exists(Path))
		{
			string message = "DiscordGSDKLibrary: Unable to locate any build library: " + Path;
			Log.TraceInformation(message);
			throw new Exception(message);
		}
		
		PublicAdditionalLibraries.Add(Path);
	}

	private void AddDllDependency(string Path)
	{
		if (!File.Exists(Path))
		{
			string message = "DiscordGSDKLibrary: Unable to locate any build library: " + Path;
			Log.TraceInformation(message);
			throw new Exception(message);
		}
		
		RuntimeDependencies.Add(Path);
	}
}