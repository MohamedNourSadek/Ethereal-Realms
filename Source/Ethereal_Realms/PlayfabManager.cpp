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
	playFabManager = this;
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
void APlayfabManager::UpdatePlayFabData(TMap<FString, FString>& dataToUpdate) const
{
	PlayFab::ClientModels::FUpdateUserDataRequest request;

	request.Data = dataToUpdate;

	auto onSuccess = PlayFab::UPlayFabClientAPI::FUpdateUserDataDelegate::CreateUObject(this, &APlayfabManager::OnDataUpdate);
	auto onError = PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &APlayfabManager::OnError);

	clientAPI->UpdateUserData(request, onSuccess, onError);
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

	AMyPlayerController* MyPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (result.Data.Contains(InventoryDataKey))
	{
		FString userDataString = result.Data[InventoryDataKey].Value;
		FPlayerInventoryData userData;
		FJsonObjectConverter::JsonObjectStringToUStruct(userDataString, &userData);

		MyPlayer->playerInventoryData = userData;

		for(auto element :userData.inventoryItems)
			playerInventory->StoreItem((InventoryItemType)(element.Key), element.Value);

		UE_LOG(LogTemp, Display, TEXT("Data recieved from user saved data"));
	}
	else 
	{
		TMap<FString, FString> DataToUpdate;
		FString DataString;
		FJsonObjectConverter::UStructToJsonObjectString(MyPlayer->playerInventoryData, DataString);

		DataToUpdate.Add(InventoryDataKey, DataString);
		UpdatePlayFabData(DataToUpdate);
	}
}
void APlayfabManager::OnError(const PlayFab::FPlayFabCppError& ErrorResult) const
{
	UE_LOG(LogTemp, Error, TEXT("Error %s "), *ErrorResult.ErrorMessage);
}
#pragma endregion 


