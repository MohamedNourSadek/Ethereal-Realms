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


	UPROPERTY(EditAnywhere) float pickRad = 200;
protected:
	virtual void BeginPlay() override;

public:	
	AMyPlayerController();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APickableItem* GetNearestObject() const;
	void AddItemToInventory(UInventoryItemData* item);
	void RemoveItemFromInventory(UInventoryItemData* item);
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly) bool RecieveInput = true;

private:
	UPROPERTY(EditAnywhere) TArray<UInventoryItemData*> inventoryItems;
};

