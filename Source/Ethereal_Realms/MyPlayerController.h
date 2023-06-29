// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableItem.h"
#include "GameFramework/Character.h"
#include "InventoryItemData.h"
#include "UserDataMap.h"
#include "PlayerData.h"
#include "FPlayerInventoryData.h"
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
	void AttackWithWeaponInHand();
#pragma endregion 

#pragma region Private Functions
	void UpdateUI() const;
	void StoreNearestItem();
	void StoreItem(InventoryItemType itemType);
#pragma endregion 

#pragma region Exposed Variables
	UPROPERTY(EditAnywhere) float pickRange = 200;
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly) bool RecieveInput = true;
	UPROPERTY(EditAnywhere) APickableItem* itemInHand = nullptr;
	UPROPERTY(EditAnywhere) FPlayerData playerData;
#pragma endregion

#pragma region Private Variables
public:
	UPROPERTY(EditAnywhere) FPlayerInventoryData playerInventoryData;
#pragma endregion 


#pragma region Callbacks
	void InventoryInputRecieved();    
	void CharacterUIOpenRecieved(); 
	void PickInputRecieved();
	void StoreInputRecieved();
	void DropInputRecieved();
	void AttackInputRecieved();
	UFUNCTION()
	void OnPlayerLoggedIn(FUserDataMap userData);
#pragma endregion 
};

    