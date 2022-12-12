// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DiscordTypes.h"
#include "DiscordRelationshipManager.generated.h"

// fast forwards
namespace discord
{
	class RelationshipManager;
}

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FDiscordRelationshipFilter, const FDiscordRelationship&, Relationship);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiscordOnRelationshipRefresh);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiscordOnRelationshipUpdate, const FDiscordRelationship&, Relationship);

/**
 * 
 */
UCLASS(Blueprintable)
class DISCORDGSDK_API UDiscordRelationshipManager : public UObject, public IDiscordInterface
{
	GENERATED_BODY()

public:
	virtual void Create(UDiscordCore* InCore) override;

	UFUNCTION(BlueprintCallable, Category = "Discord|Relationship")
	void Filter(FDiscordRelationshipFilter FilterCallback);
	UFUNCTION(BlueprintCallable, Category = "Discord|Relationship")
	bool Count(int32& Count);
	UFUNCTION(BlueprintCallable, Category = "Discord|Relationship")
	bool Get(int64 UserID, FDiscordRelationship& Relationship);
	UFUNCTION(BlueprintCallable, Category = "Discord|Relationship")
	bool GetAt(int32 Index, FDiscordRelationship& Relationship);

	UPROPERTY(BlueprintAssignable, Category = "Discord|Relationship")
	FDiscordOnRelationshipRefresh OnRelationshipRefresh;
	UPROPERTY(BlueprintAssignable, Category = "Discord|Relationship")
	FDiscordOnRelationshipUpdate OnRelationshipUpdate;
};