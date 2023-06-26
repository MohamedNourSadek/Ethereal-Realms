#pragma once

#include "../PublicStatics.h"
#include "CoreMinimal.h"
#include "FPlayerInventoryData.generated.h"

USTRUCT()
struct ETHEREAL_REALMS_API FPlayerInventoryData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<int , int> inventoryItems;
};
