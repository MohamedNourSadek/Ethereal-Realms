// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "WeaponBase.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"

extern UInventory* playerInventory = nullptr;

void UInventory::OnStart(UCanvasPanel* dropGPanel, UCanvasPanel* pressEPanel, UCanvasPanel* pressRPanel, UCanvasPanel* gameplayPanel, UCanvasPanel* inventoryPanel, TArray<UInventoryUIItem*> items)
{
	playerInventory = this;
	
	PressEPanel = pressEPanel;
	PressRPanel = pressRPanel;
	DropGPanel = dropGPanel;
	GameplayPanel = gameplayPanel;
	InventoryPanel = inventoryPanel;
	InventoryUIItems = items;

	for (UInventoryUIItem* item : items)
		item->OnStart();

	MyPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UInventory::StoreItem()
{
	if(MyPlayer != nullptr)
	{
		APickableItem* nearestObj = MyPlayer->GetNearestObject();

		if(nearestObj != nullptr)
		{
			StoreItemInUI(nearestObj);
			nearestObj->Destroy();
		}
	}
	else
	{
		MyPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		StoreItem();
	}
}
void UInventory::PickItem()
{
	if(MyPlayer != nullptr)
	{
		APickableItem* nearestObj = MyPlayer->GetNearestObject();

		if(nearestObj != nullptr)
		{
			nearestObj->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(false);
			nearestObj->FindComponentByClass<UStaticMeshComponent>()->SetCollisionProfileName("NoCollision", true);
			Cast<APickableItem>(nearestObj)->isPicked = true;
			nearestObj->AttachToComponent(Cast<ACharacter>(MyPlayer)->GetMesh() ,FAttachmentTransformRules::SnapToTargetNotIncludingScale,"hand_lSocket");
			MyPlayer->itemInHand = nearestObj;
		}
	}
	else
	{
		MyPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		PickItem();
	}
}
void UInventory::DropItem(InventoryItemType itemType)
{
	const int mySlot = GetSlot(itemType);
	const int currentAmount = FCString::Atoi(*InventoryUIItems[mySlot]->amountText->GetText().ToString());

	if(currentAmount > 1)
	{
		InventoryUIItems[mySlot]->amountText->SetText(FText::FromString(FString::FromInt(currentAmount-1)));
	}
	else if(currentAmount == 1)
	{
		InventoryUIItems[mySlot]->myImage->SetBrushFromTexture(nullptr, false);
		InventoryUIItems[mySlot]->amountText->SetText(FText::FromString(FString("")));
		InventoryUIItems[mySlot]->itemButton->SetIsEnabled(false);
		InventoryUIItems[mySlot]->isUsed = false;
		InventoryUIItems[mySlot]->myType =  InventoryItemType::Empty;
	}

	MyPlayer->RemoveItemDataFromInventory(itemType);
	
	FVector spawnLocaiton = MyPlayer->GetActorLocation() + (100*MyPlayer->GetActorForwardVector());
	FRotator spawnRotation = MyPlayer->GetActorRotation();

	
	if(itemType == InventoryItemType::Cube)
		GetWorld()->SpawnActor(cube, &spawnLocaiton, &spawnRotation);
	else if(itemType == InventoryItemType::Sword)
		GetWorld()->SpawnActor(sword, &spawnLocaiton, &spawnRotation);

}
void UInventory::ToggleInventoryState()
{
	if(MyPlayer != nullptr)
	{
		if(GameplayPanel->GetVisibility() == ESlateVisibility::Visible)
		{
			GameplayPanel->SetVisibility(ESlateVisibility::Hidden);
			InventoryPanel->SetVisibility(ESlateVisibility::Visible);
			MyPlayer->RecieveInput = false;   
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		}
		else
		{
			GameplayPanel->SetVisibility(ESlateVisibility::Visible);
			InventoryPanel->SetVisibility(ESlateVisibility::Hidden);
			MyPlayer->RecieveInput = true;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
		}
	}
	else
	{
		MyPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		ToggleInventoryState();
	}
}
void UInventory::SetPickUIState(bool state) const
{
	if(PressEPanel != nullptr)
	{
		if(state)
			PressEPanel->SetVisibility(ESlateVisibility::Visible);
		else
			PressEPanel->SetVisibility(ESlateVisibility::Hidden);
	}

	if(PressRPanel != nullptr)
	{
		if(state)
			PressRPanel->SetVisibility(ESlateVisibility::Visible);
		else
			PressRPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UInventory::StoreItemInUI(APickableItem* itemToStore)
{
	InventoryItemType itemType = InventoryItemType::Empty;
			
	if(itemToStore->Tags.Contains("Cube"))
		itemType = InventoryItemType::Cube;
	else if(itemToStore->Tags.Contains("Sword"))
		itemType = InventoryItemType::Sword;

	int unUsedSlot = GetSlot(itemType);
	const int itemsCount = MyPlayer->AddItemDataToInventory(itemType);
			
	InventoryUIItems[unUsedSlot]->myImage->SetBrushFromTexture(GetTexture(itemType), true);
	InventoryUIItems[unUsedSlot]->amountText->SetText(FText::FromString(FString::FromInt(itemsCount)));
	InventoryUIItems[unUsedSlot]->itemButton->SetIsEnabled(true);
	InventoryUIItems[unUsedSlot]->myType = itemType;
	InventoryUIItems[unUsedSlot]->isUsed = true;
}

int UInventory::GetSlot(InventoryItemType type)
{
	int alreadyExists = -1;
	int emptySlot = -1;
	
	for(int i = 0; i < InventoryUIItems.Num(); i++)
		if(InventoryUIItems[i]->myType == type)
			alreadyExists = i;
		else if(InventoryUIItems[i]->myType == InventoryItemType::Empty && emptySlot == -1)
			emptySlot = i;

	if(alreadyExists != -1)
		return alreadyExists;
	else
		return emptySlot;
}
UTexture2D* UInventory::GetTexture(InventoryItemType type)
{
	if(type == InventoryItemType::Cube)
		return textures[0];
	else if(type == InventoryItemType::Sword)
		return textures[1];
	else
		return textures[0];
}
