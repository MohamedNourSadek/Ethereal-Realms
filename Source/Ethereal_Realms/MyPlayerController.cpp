// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "MyPlayerController.h"
#include "Inventory.h"
#include "WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"

extern UInventory* playerInventory = nullptr;

#pragma region Unreal Delegates
AMyPlayerController::AMyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	playerData = NewObject<UPlayerData>();
}
void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateUI();

	for(auto element : playerInventoryData.inventoryItems)
		UE_LOG(LogTemp, Display, TEXT("%d"), element.Key);
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
		int weaponDamage = weapon->weaponData->baseDamage + playerData->power + playerData->swordsmanship;
		UE_LOG(LogTemp, Display, TEXT("Attack with %s with damage %d"), *itemInHand->itemName, weaponDamage);
	}
}
#pragma endregion

#pragma 
void AMyPlayerController::PickInputRecieved()
{
	playerInventory->PickItem();
}
void AMyPlayerController::StoreInputRecieved()
{
	playerInventory->StoreItem();
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
