// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayfabManager.h"
#include "Core/PlayFabClientAPI.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "FPlayerInventoryData.h"
#include "Inventory.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

extern UInventory* playerInventory = nullptr;
extern APlayfabManager* playFabManager = nullptr;

#pragma region Unreal Functions
APlayfabManager::APlayfabManager()
{
	PrimaryActorTick.bCanEverTick = true;
	playFabManager = this;
}
void APlayfabManager::BeginPlay()
{
	Super::BeginPlay();

	InitializePlayfab();
	LoginWithCustomID();
}
void APlayfabManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timeSinceStartUp += DeltaTime;
	UpdateDataPeriodically();
}
#pragma endregion

#pragma region Public Fucntions
void APlayfabManager::UpdateDataPeriodically()
{
	if((((int)timeSinceStartUp/ UpdateEverySecond) != lastSecondUpdated) && DataToUpdate.Num() != 0)
	{
		lastSecondUpdated = ((int)(timeSinceStartUp/ UpdateEverySecond));

		PlayFab::ClientModels::FUpdateUserDataRequest request;
		request.Data = DataToUpdate;

		auto onSuccess = PlayFab::UPlayFabClientAPI::FUpdateUserDataDelegate::CreateUObject(this, &APlayfabManager::OnDataUpdate);
		auto onError = PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &APlayfabManager::OnError);

		clientAPI->UpdateUserData(request, onSuccess, onError);

		DataToUpdate.Reset();

		UE_LOG(LogTemp, Error, TEXT("Updating .. %f"), timeSinceStartUp);
	}
}
void APlayfabManager::InitializePlayfab()
{
	GetMutableDefault<UPlayFabRuntimeSettings>()->TitleId = TEXT("6C634");
	clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
}
void APlayfabManager::LoginWithCustomID()
{
	PlayFab::ClientModels::FLoginWithCustomIDRequest request;
	
	request.CustomId = FGenericPlatformMisc::GetHashedMacAddressString();
	request.CreateAccount = true;
	
	clientAPI->LoginWithCustomID(
		request,
		PlayFab::UPlayFabClientAPI::FLoginWithCustomIDDelegate::CreateUObject(this, &APlayfabManager::OnSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &APlayfabManager::OnError)
	);
}
void APlayfabManager::UpdatePlayFabData(TMap<FString, FString>& dataToUpdate)
{
	for(auto& element : dataToUpdate)
	{
		if(DataToUpdate.Contains(element.Key))
		{
			DataToUpdate[element.Key] = element.Value;
		}
		else
		{
			DataToUpdate.Add(element.Key, element.Value);
		}

		UE_LOG(LogTemp, Warning, TEXT("Requesting To Update .. %f"), timeSinceStartUp);

	}

}

#pragma endregion 

#pragma region CallBacks

void APlayfabManager::OnSuccess(const PlayFab::ClientModels::FLoginResult& LoginResult) const
{
	auto playerID = LoginResult.PlayFabId;
	UE_LOG(LogTemp, Warning, TEXT("You've logged in successfully, you playfab ID %s"), *playerID);
	
	PlayFab::ClientModels::FGetUserDataRequest request;
	
	auto onSuccess = PlayFab::UPlayFabClientAPI::FGetUserDataDelegate::CreateUObject(this, &APlayfabManager::OnDataRetrieved);
	auto onError = PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &APlayfabManager::OnError);
	
	clientAPI->GetUserData(request,onSuccess, onError);
}
void APlayfabManager::OnDataUpdate(const PlayFab::ClientModels::FUpdateUserDataResult& result) const
{
	UE_LOG(LogTemp, Warning, TEXT("DATA Updated"));
}
void APlayfabManager::OnDataRetrieved(const PlayFab::ClientModels::FGetUserDataResult& result) const
{
	OnUserLoggedInEvent.Broadcast(FUserDataMap(result.Data));
}
void APlayfabManager::OnError(const PlayFab::FPlayFabCppError& ErrorResult) const
{
	UE_LOG(LogTemp, Error, TEXT("Error %s "), *ErrorResult.ErrorMessage);
}
#pragma endregion 


