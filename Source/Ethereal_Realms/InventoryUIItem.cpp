// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUIItem.h"

#include "Ethereal_RealmsGameModeBase.h"

void UInventoryUIItem::OnStart()
{
	if(itemButton != nullptr)
	{
		itemButton->OnClicked.AddDynamic(this, &UInventoryUIItem::OnButtonClicked);
	}
}

void UInventoryUIItem::OnButtonClicked()
{
	Cast<AEthereal_RealmsGameModeBase>(GetWorld()->GetAuthGameMode())->myInventory->DropItem(this);
}
