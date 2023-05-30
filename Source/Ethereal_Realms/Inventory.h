// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "InventoryUIItem.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class ETHEREAL_REALMS_API UInventory : public UUserWidget
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable) void SetInventoryState(UCanvasPanel* GamePlayPanel, UCanvasPanel* Inventory);
	UFUNCTION(BlueprintCallable) void PickItem();
	UFUNCTION(BlueprintCallable) void OnStart(UCanvasPanel* pressEPanel, TArray<UInventoryUIItem*> items);

	UPROPERTY(EditAnywhere) TSubclassOf<AActor> cube;
	void SetPickUIState(bool pressEPanel);
	void DropItem(UInventoryUIItem* item);
	
	AMyPlayerController* myPlayer = nullptr;
	UCanvasPanel* PressEPanel;
	TArray<UInventoryUIItem*> inventoryUIItems;
	
	UPROPERTY(EditAnywhere) TArray<UTexture2D*> textures;
};
