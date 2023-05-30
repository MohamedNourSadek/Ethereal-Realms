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
	void OnStart();
	UFUNCTION(BlueprintCallable) void OnButtonClicked();
	
	UPROPERTY(EditAnywhere) UImage* myImage = nullptr;
	UPROPERTY(EditAnywhere) UTextBlock* amountText = nullptr;
	UPROPERTY(EditAnywhere) UButton* itemButton = nullptr;
	InventoryItemType myType = InventoryItemType::Empty;
};
 