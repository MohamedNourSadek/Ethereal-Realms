// Fill out your copyright notice in the Description page of Project Settings.


#include "UserDataMap.h"


FUserDataMap::FUserDataMap()
{
}

FUserDataMap::FUserDataMap(TMap<FString, PlayFab::ClientModels::FUserDataRecord> userData)
{
	UserDataMap = userData;
}

