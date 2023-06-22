// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayfabManager.h"
#include "Core/PlayFabClientAPI.h"

#pragma region Unreal Functions
APlayfabManager::APlayfabManager()
{
	PrimaryActorTick.bCanEverTick = true;

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

}
#pragma endregion

#pragma region Public Fucntions
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
	UE_LOG(LogTemp, Warning, TEXT("DATA RECIEVED"));

	if(result.Data.Contains(InventoryDataKey))
		UE_LOG(LogTemp, Display, TEXT("Data recieved from user saved data"))
	else 
	{
		PlayFab::ClientModels::FUpdateUserDataRequest request; 
		TMap<FString, FString> x;
		x.Add(InventoryDataKey, "");
		x[InventoryDataKey] = "";
		request.Data = x;
		auto onSuccess = PlayFab::UPlayFabClientAPI::FUpdateUserDataDelegate::CreateUObject(this, &APlayfabManager::OnDataUpdate);
		auto onError = PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &APlayfabManager::OnError);
	
		clientAPI->UpdateUserData(request, onSuccess, onError);
	}
}
void APlayfabManager::OnError(const PlayFab::FPlayFabCppError& ErrorResult) const
{
	UE_LOG(LogTemp, Error, TEXT("Error %s "), *ErrorResult.ErrorMessage);
}
#pragma endregion 


