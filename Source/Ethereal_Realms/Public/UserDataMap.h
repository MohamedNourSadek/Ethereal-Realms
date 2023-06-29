// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayFabClientDataModels.h"
#include "UserDataMap.generated.h"

USTRUCT()
struct ETHEREAL_REALMS_API FUserDataMap
{
	GENERATED_BODY()

	FUserDataMap();
	FUserDataMap(TMap<FString, PlayFab::ClientModels::FUserDataRecord> userData);
	TMap<FString, PlayFab::ClientModels::FUserDataRecord> UserDataMap;
};

