// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PublicStatics.h"
#include "InventoryItemData.generated.h"

USTRUCT()
struct ETHEREAL_REALMS_API FInventoryItemData
{
	GENERATED_BODY()

public:
	InventoryItemType ItemType;
};

