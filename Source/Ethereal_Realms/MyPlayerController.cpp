// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "MyPlayerController.h"
#include "Inventory.h"
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
}
void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateUI();
}
void AMyPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Store", IE_Pressed, this, &AMyPlayerController::StoreInputRecieved);
	PlayerInputComponent->BindAction("Pick", IE_Pressed, this, &AMyPlayerController::PickInputRecieved);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMyPlayerController::InventoryInputRecieved);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AMyPlayerController::DropInputRecieved);
}
#pragma endregion


#pragma region Functions 
void AMyPlayerController::UpdateUI() const
{
	const APickableItem* nearestObject = GetNearestObject();

	if(playerInventory != nullptr)
	{
		if(nearestObject != nullptr )
			playerInventory->SetPickUIState(true);
		else
			playerInventory->SetPickUIState(false);
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
	UInventoryItemData* myItem = nullptr;
	
	for(const auto &item : inventoryItems)
	{
		if(item.Key->ItemType == itemType)
			myItem = item.Key;
	}

	if(myItem != nullptr)
	{
		inventoryItems[myItem] += 1;
		return inventoryItems[myItem];
	}
	else
	{
		UInventoryItemData* newItem = NewObject<UInventoryItemData>();
		newItem->ItemType = itemType;
		inventoryItems.Add(newItem, 1);

		return 1;
	}

}
int AMyPlayerController::RemoveItemDataFromInventory(InventoryItemType itemType)
{
	UInventoryItemData* myItem = nullptr;

	for(const auto &item : inventoryItems)
	{
		if(item.Key->ItemType == itemType)
			myItem = item.Key;
	}

	if(myItem != nullptr)
	{
		if(inventoryItems[myItem] > 1)
		{
			inventoryItems[myItem] -= 1;
			return inventoryItems[myItem]; 
		}
		else
		{
			inventoryItems.Remove(myItem);
			return 0;
		}
	}
	else
	{
		return 0;
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
