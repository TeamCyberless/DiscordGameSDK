// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FDiscordGSDKModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

#if WITH_EDITOR
	void OnPostEngineInit();
	void OnPreExit();
#endif

private:
	/** Handle to the test dll we will load. */
	void* DiscordGSDKLibraryHandle = nullptr;

	/** StartupModule is covered with defines, these functions are the place to put breakpoints. */
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void FreeDependency(void*& Handle);
};
