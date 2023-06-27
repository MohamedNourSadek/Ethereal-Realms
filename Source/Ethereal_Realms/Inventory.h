// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryUIItem.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/VerticalBox.h"
#include "Inventory.generated.h"
UCLASS()
class ETHEREAL_REALMS_API UInventory : public UUserWidget
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable) void OnStart(UCanvasPanel* dropGPanel, UCanvasPanel* pressEPanel, UCanvasPanel* pressRPanel, UCanvasPanel* pressLMPanel, UCanvasPanel* GamePlayPanel, UCanvasPanel* InventoryPanel, UCanvasPanel* characterCanvas,USlider* PowerSlider, USlider* swordsmanshipSlider, USlider* tacticsSlider, TArray<UInventoryUIItem*> items);
	void ToggleInventoryState();
	void ToggleCharacterState();
	void PickItem();

#pragma region Functions
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> cube;
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> sword;
	UFUNCTION(BlueprintCallable) void OnCharacterParametersChanged(float newValue);
	void UpdateUI(bool nearObjectExist, bool objectInHandExist) const;
	void DropItem(InventoryItemType itemType);
	void StoreItem(InventoryItemType type, int itemsCount);
	int GetSlot(InventoryItemType type);
	UTexture2D* GetTexture(InventoryItemType type);
#pragma endregion

#pragma region Variables
	UPROPERTY(EditAnywhere) TArray<UTexture2D*> textures;
	UPROPERTY(EditAnywhere) UVerticalBox* buttonsParent;
	AMyPlayerController* MyPlayer = nullptr;
	UPROPERTY(EditAnywhere) UCanvasPanel* PressEPanel = nullptr;
	UPROPERTY(EditAnywhere) UCanvasPanel* PressRPanel = nullptr;
	UPROPERTY(EditAnywhere) UCanvasPanel* DropGPanel = nullptr;
	UPROPERTY(EditAnywhere) UCanvasPanel* PressLMPanel = nullptr;
	UPROPERTY(EditAnywhere) UCanvasPanel* GameplayPanel= nullptr;
	UPROPERTY(EditAnywhere) UCanvasPanel* InventoryPanel= nullptr;
	UPROPERTY(EditAnywhere) UCanvasPanel* CharacterCanvas = nullptr;
	UPROPERTY(EditAnywhere) USlider* PowerSlider = nullptr;
	UPROPERTY(EditAnywhere) USlider* SwordsManShipSlider = nullptr;
	UPROPERTY(EditAnywhere) USlider* TacticsSlider = nullptr;
	UPROPERTY(EditAnywhere) TArray<UInventoryUIItem*> InventoryUIItems;
#pragma endregion
	
};
