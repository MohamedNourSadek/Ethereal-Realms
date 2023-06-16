// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableItem.h"
#include "GameFramework/Character.h"
#include "InventoryItemData.h"
#include "MyPlayerController.generated.h"




UCLASS()
class ETHEREAL_REALMS_API AMyPlayerController : public ACharacter
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

public:	

	AMyPlayerController();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Public Functions
	APickableItem* GetNearestObject() const;
	int AddItemDataToInventory(InventoryItemType itemType);
	int RemoveItemDataFromInventory(InventoryItemType itemType);
#pragma endregion 

#pragma region Private Functions
	void UpdateUI() const;
#pragma endregion 

#pragma region Exposed Variables
	UPROPERTY(EditAnywhere) float pickRange = 200;
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly) bool RecieveInput = true;
	UPROPERTY(EditAnywhere) APickableItem* itemInHand = nullptr;
#pragma endregion

#pragma region Private Variables
	TMap<UInventoryItemData*, int> inventoryItems;
#pragma endregion 


#pragma region Input Callbacks
	void InventoryInputRecieved();
	void PickInputRecieved();
	void StoreInputRecieved();
	void DropInputRecieved();
#pragma endregion 
};

