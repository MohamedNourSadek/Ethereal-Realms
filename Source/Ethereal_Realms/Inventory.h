// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryUIItem.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

UCLASS()
class ETHEREAL_REALMS_API UInventory : public UUserWidget
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable) void OnStart(UCanvasPanel* PressEPanel, UCanvasPanel* GamePlayPanel, UCanvasPanel* InventoryPanel,TArray<UInventoryUIItem*> items);
	void ToggleInventoryState();
	void PickItem();

	UPROPERTY(EditAnywhere) TSubclassOf<AActor> cube;
	void SetPickUIState(bool pressEPanel) const;
	void DropItem(UInventoryUIItem* item);
	int GetSlot(InventoryItemType type);
	UTexture2D* GetTexture(InventoryItemType type);
	
	AMyPlayerController* MyPlayer = nullptr;
	UCanvasPanel* PressEPanel = nullptr;
	UCanvasPanel* GameplayPanel= nullptr;
	UCanvasPanel* InventoryPanel= nullptr;
	TArray<UInventoryUIItem*> InventoryUIItems; 
	
	UPROPERTY(EditAnywhere) TArray<UTexture2D*> textures;
};
