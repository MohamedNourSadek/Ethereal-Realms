// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "UserDataMap.h"
#include "PlayfabManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserLoggedIn, FUserDataMap, UserData);

UCLASS()
class ETHEREAL_REALMS_API APlayfabManager : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	int UpdateEverySecond = 4;
	TMap<FString, FString> DataToUpdate;
	FOnUserLoggedIn OnUserLoggedInEvent;

	APlayfabManager();
	FString InventoryDataKey = "InventoryDataKey";
	FString PlayerDataKey = "CharacterDayKey";
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void UpdatePlayFabData(TMap<FString, FString>& dataToUpdate);
	void OnSuccess(const PlayFab::ClientModels::FLoginResult& LoginResult) const;
	void OnError(const PlayFab::FPlayFabCppError& ErrorResult) const;
	void OnDataRetrieved(const PlayFab::ClientModels::FGetUserDataResult& request) const;
	void InitializePlayfab();
	void LoginWithCustomID();
	void OnDataUpdate(const PlayFab::ClientModels::FUpdateUserDataResult& result) const;
private:
	UPROPERTY(EditAnywhere)
	int lastSecondUpdated = 0;
	UPROPERTY(EditAnywhere)
	float timeSinceStartUp = 0.0;
	void UpdateDataPeriodically();
	PlayFabClientPtr clientAPI = nullptr;
};
