// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include <string>

#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"

void UInventory::OnStart(UCanvasPanel* pressEPanel, TArray<UInventoryUIItem*> items)
{
	PressEPanel = pressEPanel;
	inventoryUIItems = items;

	for (UInventoryUIItem* item : items)
		item->OnStart();
}
void UInventory::PickItem()
{
	UE_LOG(LogTemp, Display, TEXT("Pick Item"));
	if(myPlayer != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Player Exists"));

		APickableItem* nearestObj = myPlayer->GetNearestObject();
		
		if(nearestObj != nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("Near Object Exists"));

			if(nearestObj->Tags.Contains("Cube"))
			{
				UInventoryItemData* Item = NewObject<UInventoryItemData>();
				Item->ItemType = Cube;
				const int itemsCount = myPlayer->AddItemToInventory(Item);
				
				inventoryUIItems[0]->myImage->SetBrushFromTexture(textures[0], true);
				inventoryUIItems[0]->amountText->SetText(FText::FromString(FString::FromInt(itemsCount)));
				inventoryUIItems[0]->itemButton->SetIsEnabled(true);
				inventoryUIItems[0]->myType = InventoryItemType::Cube;
			}

			nearestObj->Destroy();
		}
	}
	else
	{
		myPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		PickItem();
	}
}
void UInventory::DropItem(UInventoryUIItem* item)
{
	int currentAmount = FCString::Atoi(*inventoryUIItems[0]->amountText->GetText().ToString());

	if(currentAmount > 1)
		inventoryUIItems[0]->amountText->SetText(FText::FromString(FString::FromInt(currentAmount-1)));
	else
	{
		inventoryUIItems[0]->myImage->SetBrushFromTexture(nullptr, false);
		inventoryUIItems[0]->amountText->SetText(FText::FromString(FString("")));
		inventoryUIItems[0]->itemButton->SetIsEnabled(true);
		//myPlayer->RemoveItemFromInventory();
		if(item->myType == InventoryItemType::Cube)
			GetWorld()->SpawnActor(cube,&myPlayer->GetTransform());
	}
}
void UInventory::SetInventoryState(UCanvasPanel* GamePlayPanel, UCanvasPanel* Inventory)
{
	if(myPlayer != nullptr)
	{
		if(GamePlayPanel->GetVisibility() == ESlateVisibility::Visible)
		{
			GamePlayPanel->SetVisibility(ESlateVisibility::Hidden);
			Inventory->SetVisibility(ESlateVisibility::Visible);
			myPlayer->RecieveInput = false;   
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		}
		else
		{
			GamePlayPanel->SetVisibility(ESlateVisibility::Visible);
			Inventory->SetVisibility(ESlateVisibility::Hidden);
			myPlayer->RecieveInput = true;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
		}
	}
	else
	{
		myPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		SetInventoryState(GamePlayPanel, Inventory);
	}
}
void UInventory::SetPickUIState(bool state)
{
	if(state)
		PressEPanel->SetVisibility(ESlateVisibility::Visible);
	else
		PressEPanel->SetVisibility(ESlateVisibility::Hidden);
}
