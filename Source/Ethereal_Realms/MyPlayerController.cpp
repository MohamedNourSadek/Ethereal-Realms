// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "MyPlayerController.h"
#include "Inventory.h"
#include "JsonObjectConverter.h"
#include "PlayfabManager.h"
#include "WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"

extern UInventory* playerInventory = nullptr;
extern APlayfabManager* playFabManager = nullptr;

bool initialized = false;

#pragma region Unreal Delegates
AMyPlayerController::AMyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (playFabManager == nullptr)
	{
		playFabManager->OnUserLoggedInEvent.AddDynamic(this, &AMyPlayerController::OnPlayerLoggedIn);
		initialized = true;
	}
}
void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateUI();

	if (initialized == false && playFabManager != nullptr)
	{
		playFabManager->OnUserLoggedInEvent.AddDynamic(this, &AMyPlayerController::OnPlayerLoggedIn);
		initialized = true;
	}

}
void AMyPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Store", IE_Pressed, this, &AMyPlayerController::StoreInputRecieved);
	PlayerInputComponent->BindAction("Pick", IE_Pressed, this, &AMyPlayerController::PickInputRecieved);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMyPlayerController::InventoryInputRecieved);
	PlayerInputComponent->BindAction("CharacterUI", IE_Pressed, this, &AMyPlayerController::CharacterUIOpenRecieved);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AMyPlayerController::DropInputRecieved);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyPlayerController::AttackInputRecieved);
}
#pragma endregion


#pragma region Functions 
void AMyPlayerController::UpdateUI() const
{
	const APickableItem* nearestObject = GetNearestObject();

	if(playerInventory != nullptr)
	{
		bool nearObjectExits = (nearestObject != nullptr) ? true : false;
		bool objectInHandExsits = (itemInHand != nullptr) ? true : false;
		playerInventory->UpdateUI(nearObjectExits, objectInHandExsits);
	}
}
APickableItem* AMyPlayerController::GetNearestObject() const
{
	TArray<FHitResult> HitArray;
	const FVector location = this->GetActorLocation();
	const TArray<AActor*> exceptionActors {GetOwner()};

	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		location,
		location,
		pickRange,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldDynamic),
		false,
		exceptionActors,
		EDrawDebugTrace::None,
		HitArray,
		true);

	for(FHitResult& element : HitArray)
	{
		if(element.GetActor()->Tags.Contains("Pickable"))
		{
			APickableItem* item = Cast<APickableItem>(element.GetActor());
			
			if(item->isPicked == false && itemInHand == nullptr)
				return item;
		}
	}

	return nullptr;
}
int AMyPlayerController::AddItemDataToInventory(InventoryItemType itemType)
{
	InventoryItemType myItem = InventoryItemType::Empty;

	for(const auto &item : playerInventoryData.inventoryItems)
	{
		if (item.Key == itemType)
		{
			myItem = (InventoryItemType)(item.Key);
		}
	}

	if(myItem != InventoryItemType::Empty)
	{
		playerInventoryData.inventoryItems[myItem] += 1;
		return playerInventoryData.inventoryItems[myItem];
	}
	else
	{
		InventoryItemType newItem = itemType;
		playerInventoryData.inventoryItems.Add(newItem, 1);

		return 1;
	}
}
int AMyPlayerController::RemoveItemDataFromInventory(InventoryItemType itemType)
{
	InventoryItemType myItem = InventoryItemType::Empty;

	for(const auto &item : playerInventoryData.inventoryItems)
	{
		if (item.Key == itemType)
			myItem = (InventoryItemType)(item.Key);
	}

	if(myItem != InventoryItemType::Empty)
	{
		if(playerInventoryData.inventoryItems[myItem] > 1)
		{
			playerInventoryData.inventoryItems[myItem] -= 1;
			return playerInventoryData.inventoryItems[myItem];
		}
		else
		{
			playerInventoryData.inventoryItems.Remove(myItem);
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
void AMyPlayerController::AttackWithWeaponInHand()
{
	if(itemInHand != nullptr && itemInHand->Tags.Contains("Weapon"))
	{
		AWeaponBase* weapon = Cast<AWeaponBase>(itemInHand);
		int weaponDamage = weapon->weaponData->baseDamage + playerData.power + playerData.swordsmanship;
		UE_LOG(LogTemp, Display, TEXT("Attack with %s with damage %d"), *itemInHand->itemName, weaponDamage);
	}
}
void AMyPlayerController::StoreItem(InventoryItemType itemType)
{
	const int itemsCount = AddItemDataToInventory(itemType);
	playerInventory->StoreItem(itemType, itemsCount);
}
void AMyPlayerController::StoreNearestItem()
{
	APickableItem* nearestObj = GetNearestObject();

	if (nearestObj != nullptr)
	{
		InventoryItemType itemType = InventoryItemType::Empty;

		if (nearestObj->Tags.Contains("Cube"))
			itemType = InventoryItemType::Cube;
		else if (nearestObj->Tags.Contains("Sword"))
			itemType = InventoryItemType::Sword;

		const int itemsCount = AddItemDataToInventory(itemType);
		playerInventory->StoreItem(itemType, itemsCount);
		nearestObj->Destroy();


		TMap<FString, FString> DataToUpdate;
		FString DataString;
		FJsonObjectConverter::UStructToJsonObjectString(playerInventoryData, DataString);
		DataToUpdate.Add(playFabManager->InventoryDataKey, DataString);
		playFabManager->UpdatePlayFabData(DataToUpdate);
	}
}

#pragma endregion

#pragma
void AMyPlayerController::OnPlayerLoggedIn(FUserDataMap userData)
{
	if (userData.UserDataMap.Contains(playFabManager->InventoryDataKey))
	{
		FString userDataString = userData.UserDataMap[playFabManager->InventoryDataKey].Value;
		FPlayerInventoryData userData;
		FJsonObjectConverter::JsonObjectStringToUStruct(userDataString, &userData);

		playerInventoryData = userData;

		for (auto &element : userData.inventoryItems)
			playerInventory->StoreItem((InventoryItemType)(element.Key), element.Value);

		UE_LOG(LogTemp, Warning, TEXT("Data recieved from user saved data"));
	}
	else
	{
		TMap<FString, FString> DataToUpdate;
		FString DataString;
		FJsonObjectConverter::UStructToJsonObjectString(playerInventoryData, DataString);

		DataToUpdate.Add(playFabManager->InventoryDataKey, DataString);
		playFabManager->UpdatePlayFabData(DataToUpdate);
	}

	if(userData.UserDataMap.Contains(playFabManager->PlayerDataKey))
	{
		FString userDataString = userData.UserDataMap[playFabManager->PlayerDataKey].Value;
		FPlayerData userData;
		FJsonObjectConverter::JsonObjectStringToUStruct(userDataString, &userData);

		playerData = userData;

		UE_LOG(LogTemp, Warning, TEXT("Data recieved from user saved data"));
	}
	else
	{
		TMap<FString, FString> DataToUpdate;
		FString DataString;
		FJsonObjectConverter::UStructToJsonObjectString(playerData, DataString);

		DataToUpdate.Add(playFabManager->PlayerDataKey, DataString);
		playFabManager->UpdatePlayFabData(DataToUpdate);
	}
}

void AMyPlayerController::PickInputRecieved()
{
	playerInventory->PickItem();
}
void AMyPlayerController::StoreInputRecieved()
{
	StoreNearestItem();
}
void AMyPlayerController::InventoryInputRecieved()
{
	playerInventory->ToggleInventoryState();
}
void AMyPlayerController::CharacterUIOpenRecieved()
{
	playerInventory->ToggleCharacterState();
}
void AMyPlayerController::DropInputRecieved()
{
	if(itemInHand != nullptr)
	{
		itemInHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		itemInHand->isPicked = false;
		itemInHand->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
		itemInHand->FindComponentByClass<UStaticMeshComponent>()->SetCollisionProfileName("PhysicsActor", true);
		itemInHand = nullptr;
	}
}
void AMyPlayerController::AttackInputRecieved()
{
	AttackWithWeaponInHand();
}
