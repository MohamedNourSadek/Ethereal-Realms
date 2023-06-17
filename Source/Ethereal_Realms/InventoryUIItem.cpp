// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUIItem.h"
#include "Ethereal_RealmsGameModeBase.h"

extern UInventory* playerInventory;

void UInventoryUIItem::OnStart()
{
	if(itemButton != nullptr)
	{
		itemButton->OnClicked.AddDynamic(this, &UInventoryUIItem::OnButtonClicked);
	}
}        

void UInventoryUIItem::OnButtonClicked()
{
	playerInventory->DropItem(myType);
}
