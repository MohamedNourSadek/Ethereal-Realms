// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include <string>

#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"

extern UInventory* playerInventory = nullptr;

void UInventory::OnStart(UCanvasPanel* pressEPanel, UCanvasPanel* gameplayPanel, UCanvasPanel* inventoryPanel, TArray<UInventoryUIItem*> items)
{
	playerInventory = this;
	
	PressEPanel = pressEPanel;
	GameplayPanel = gameplayPanel;
	InventoryPanel = inventoryPanel;
	InventoryUIItems = items;

	for (UInventoryUIItem* item : items)
		item->OnStart();
}
void UInventory::PickItem()
{
	UE_LOG(LogTemp, Display, TEXT("Pick Item"));
	
	if(MyPlayer != nullptr)
	{
		APickableItem* nearestObj = MyPlayer->GetNearestObject();
		
		if(nearestObj != nullptr)
		{
			UInventoryItemData* Item = NewObject<UInventoryItemData>();

			
			if(nearestObj->Tags.Contains("Cube"))
				Item->ItemType = InventoryItemType::Cube;
			else if(nearestObj->Tags.Contains("Sword"))
				Item->ItemType = InventoryItemType::Sword;

			int unUsedSlot = GetSlot(Item->ItemType);
			const int itemsCount = MyPlayer->AddItemToInventory(Item);
			
			InventoryUIItems[unUsedSlot]->myImage->SetBrushFromTexture(GetTexture(Item->ItemType), true);
			InventoryUIItems[unUsedSlot]->amountText->SetText(FText::FromString(FString::FromInt(itemsCount)));
			InventoryUIItems[unUsedSlot]->itemButton->SetIsEnabled(true);
			InventoryUIItems[unUsedSlot]->myType = Item->ItemType;
			InventoryUIItems[unUsedSlot]->isUsed = true;

			nearestObj->Destroy();
		}
	}
	else
	{
		MyPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		PickItem();
	}
}
void UInventory::DropItem(UInventoryUIItem* item)
{
	int currentAmount = FCString::Atoi(*InventoryUIItems[0]->amountText->GetText().ToString());

	if(currentAmount > 1)
		InventoryUIItems[0]->amountText->SetText(FText::FromString(FString::FromInt(currentAmount-1)));
	else if(currentAmount == 1)
	{
		InventoryUIItems[0]->myImage->SetBrushFromTexture(nullptr, false);
		InventoryUIItems[0]->amountText->SetText(FText::FromString(FString("")));
		InventoryUIItems[0]->itemButton->SetIsEnabled(false);
		InventoryUIItems[0]->isUsed = false;
		InventoryUIItems[0]->myType =  InventoryItemType::Empty;
	}

	MyPlayer->RemoveItemFromInventory(item->myType);
	if(item->myType == InventoryItemType::Cube)
	{
		FVector spawnLocaiton = MyPlayer->GetActorLocation() + (100*MyPlayer->GetActorForwardVector());
		FRotator spawnRotation = MyPlayer->GetActorRotation();
		
		GetWorld()->SpawnActor(cube, &spawnLocaiton, &spawnRotation);
	}
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
