// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "PlayfabManager.generated.h"

UCLASS()
class ETHEREAL_REALMS_API APlayfabManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayfabManager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void OnSuccess(const PlayFab::ClientModels::FLoginResult& LoginResult) const;
	void OnError(const PlayFab::FPlayFabCppError& ErrorResult) const;


	void InitializePlayfab();
	void LoginWithCustomID();
private:
	PlayFabClientPtr clientAPI = nullptr;
};
