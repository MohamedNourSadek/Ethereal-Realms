// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PublicStatics.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryUIItem.generated.h"

/**
 * 
 */
UCLASS()
class ETHEREAL_REALMS_API UInventoryUIItem : public UCanvasPanel
{
	GENERATED_BODY()
	
public:
#pragma region Functions
	UFUNCTION(BlueprintCallable) void OnButtonClicked();
	void OnStart();
#pragma endregion 

#pragma region References
	UPROPERTY(EditAnywhere) UImage* myImage = nullptr;
	UPROPERTY(EditAnywhere) UTextBlock* amountText = nullptr;
	UPROPERTY(EditAnywhere) UButton* itemButton = nullptr;
	UPROPERTY(EditAnywhere) bool isUsed = false;
	InventoryItemType myType = InventoryItemType::Empty;
#pragma endregion
};
 