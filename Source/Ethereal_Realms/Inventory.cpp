// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"

void UInventory::OnStart(UCanvasPanel* pressEPanel)
{
	PressEPanel = pressEPanel;
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
		}
		else
		{
			GamePlayPanel->SetVisibility(ESlateVisibility::Visible);
			Inventory->SetVisibility(ESlateVisibility::Hidden);
			myPlayer->RecieveInput = true;
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
