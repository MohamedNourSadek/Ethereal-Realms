// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "JsonObjectConverter.h"
#include "WeaponBase.h"
#include "Components/CanvasPanel.h"
#include "PlayFab.h"
#include "PlayfabManager.h"
#include "Kismet/GameplayStatics.h"

extern UInventory* playerInventory = nullptr;
extern APlayfabManager* playFabManager = nullptr;

void UInventory::OnStart(UCanvasPanel* dropGPanel, UCanvasPanel* pressEPanel, UCanvasPanel* pressRPanel, UCanvasPanel* pressLMPanel, UCanvasPanel* gameplayPanel, UCanvasPanel* inventoryPanel, UCanvasPanel* characterCanvas,USlider* powerSlider, USlider* swordsmanshipSlider, USlider* tacticsSlider, TArray<UInventoryUIItem*> items)
{
	playerInventory = this;
	
	PressEPanel = pressEPanel;
	PressRPanel = pressRPanel;
	DropGPanel = dropGPanel;
	PressLMPanel = pressLMPanel;
	GameplayPanel = gameplayPanel;
	InventoryPanel = inventoryPanel;
	CharacterCanvas = characterCanvas;
	InventoryUIItems = items;
	PowerSlider = powerSlider;
	SwordsManShipSlider = swordsmanshipSlider;
	TacticsSlider = tacticsSlider;

	PowerSlider->OnValueChanged.AddDynamic(this, &UInventory::OnCharacterParametersChanged);
	SwordsManShipSlider->OnValueChanged.AddDynamic(this, &UInventory::OnCharacterParametersChanged);
	TacticsSlider->OnValueChanged.AddDynamic(this, &UInventory::OnCharacterParametersChanged);
	
	for (UInventoryUIItem* item : items)
		item->OnStart();


	
	MyPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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
		else if(InventoryPanel->GetVisibility() == ESlateVisibility::Visible)
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
void UInventory::ToggleCharacterState()
{
	if(MyPlayer != nullptr)
	{
		if(GameplayPanel->GetVisibility() == ESlateVisibility::Visible)
		{
			UpdateCharacterUIFromUserData();	
			CharacterCanvas->SetVisibility(ESlateVisibility::Visible);

			GameplayPanel->SetVisibility(ESlateVisibility::Hidden);
			MyPlayer->RecieveInput = false;   
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		}
		else if(CharacterCanvas->GetVisibility() == ESlateVisibility::Visible)
		{
			GameplayPanel->SetVisibility(ESlateVisibility::Visible);
			CharacterCanvas->SetVisibility(ESlateVisibility::Hidden);
			MyPlayer->RecieveInput = true;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
		}
	}
	else
	{
		MyPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		ToggleCharacterState();
	}
}
void UInventory::UpdateCharacterUIFromUserData()
{
	PowerSlider->SetValue(MyPlayer->playerData.power / 100.0);
	SwordsManShipSlider->SetValue(MyPlayer->playerData.swordsmanship / 100.0);
	TacticsSlider->SetValue(MyPlayer->playerData.tactics / 100.0);
}
void UInventory::UpdateUI(bool nearObjectExist, bool objectInHandExist) const
{
	if(PressEPanel != nullptr)
	{
		if(nearObjectExist)
			PressEPanel->SetIsEnabled(true);
		else
			PressEPanel->SetIsEnabled(false);
	}

	if(PressRPanel != nullptr)
	{
		if(nearObjectExist)
			PressRPanel->SetIsEnabled(true);
		else
			PressRPanel->SetIsEnabled(false);
	}

	if(DropGPanel != nullptr)
	{
		if(objectInHandExist)
			DropGPanel->SetIsEnabled(true);
		else
			DropGPanel->SetIsEnabled(false);
	}

	if(PressLMPanel != nullptr)
	{
		if(objectInHandExist)
		{
			if(MyPlayer->itemInHand->Tags.Contains("Weapon"))
			{
				PressLMPanel->SetIsEnabled(true);
			}
			else
			{
				PressLMPanel->SetIsEnabled(false);
			}
		}
		else
		{
			PressLMPanel->SetIsEnabled(false);
		}
	}
}
void UInventory::StoreItem(InventoryItemType itemType, int itemsCount)
{
	int unUsedSlot = GetSlot(itemType);

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
void UInventory::OnCharacterParametersChanged(float value)
{
	MyPlayer->playerData.power = PowerSlider->GetValue() * 100;
	MyPlayer->playerData.swordsmanship = SwordsManShipSlider->GetValue() * 100;
	MyPlayer->playerData.tactics = TacticsSlider->GetValue() * 100;

	TMap<FString, FString> DataToUpdate;
	FString DataString;
	FJsonObjectConverter::UStructToJsonObjectString(MyPlayer->playerData, DataString);
	DataToUpdate.Add(playFabManager->PlayerDataKey, DataString);
	playFabManager->UpdatePlayFabData(DataToUpdate);
}