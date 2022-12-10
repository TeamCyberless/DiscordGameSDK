// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiscordTypes.h"
#include "DiscordCore.h"

discord::Core* IDiscordInterface::GetCore() const
{
	if (Core && Core->GetCore())
	{
		return Core->GetCore();
	}

	return nullptr;
}
