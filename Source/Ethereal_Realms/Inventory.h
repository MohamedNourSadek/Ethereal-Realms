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

#pragma region Functions
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> cube;
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> sword;
	void SetPickUIState(bool pressEPanel) const;
	void DropItem(InventoryItemType itemType);
	void StoreItemInUI(APickableItem* objectToStore);
	int GetSlot(InventoryItemType type);
	UTexture2D* GetTexture(InventoryItemType type);
#pragma endregion

#pragma region Variables
	UPROPERTY(EditAnywhere) TArray<UTexture2D*> textures;
	AMyPlayerController* MyPlayer = nullptr;
	UCanvasPanel* PressEPanel = nullptr;
	UCanvasPanel* GameplayPanel= nullptr;
	UCanvasPanel* InventoryPanel= nullptr;
	TArray<UInventoryUIItem*> InventoryUIItems;
#pragma endregion
	
};
