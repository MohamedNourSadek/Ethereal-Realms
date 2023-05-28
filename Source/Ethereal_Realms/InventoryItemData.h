// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PublicStatics.h"
#include "InventoryItemData.generated.h"

UCLASS()


class ETHEREAL_REALMS_API UInventoryItemData : public UObject
{
	GENERATED_BODY()

public:
	InventoryItemType ItemType;
};

