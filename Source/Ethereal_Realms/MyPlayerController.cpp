// Fill out your copyright notice in the Description page of Project Settings.



#include "MyPlayerController.h"
#include "Ethereal_RealmsGameModeBase.h"
#include "Inventory.h"
#include "Kismet/KismetSystemLibrary.h"

extern UInventory* playerInventory = nullptr;

AMyPlayerController::AMyPlayerController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();



}
void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const APickableItem* nearestObject = GetNearestObject();

	if(playerInventory != nullptr)
	{
		if(nearestObject != nullptr )
			playerInventory->SetPickUIState(true);
		else
			playerInventory->SetPickUIState(false);
	}
}
void AMyPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Pick", IE_Pressed, this, &AMyPlayerController::PickInputRecieved);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMyPlayerController::InventoryInputRecieved);
}

APickableItem* AMyPlayerController::GetNearestObject() const
{
	TArray<FHitResult> HitArray;
	const FVector location = this->GetActorLocation();
	const TArray<AActor*> exceptionActors {GetOwner()};

	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		location,
		location,
		pickRad,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldDynamic),
		false,
		exceptionActors,
		EDrawDebugTrace::None,
		HitArray,
		true);

	for(FHitResult& element : HitArray)
	{
		if(element.GetActor()->Tags.Contains("Pickable"))
			return Cast<APickableItem>(element.GetActor());
	}

	return nullptr;
}

int AMyPlayerController::AddItemToInventory(UInventoryItemData* item)
{
	if(inventoryItems.Contains(item) == false)
	{
		inventoryItems.Add(item);
	}

	return inventoryItems.Num();
}
int AMyPlayerController::RemoveItemFromInventory(InventoryItemType type)
{
	UInventoryItemData* item = nullptr;
	
	for(int i = 0; i < inventoryItems.Num(); i++)
		if(inventoryItems[i]->ItemType == type)
			item = inventoryItems[i];

	if(item != nullptr)
		inventoryItems.Remove(item);

	return inventoryItems.Num();
}
void AMyPlayerController::PickInputRecieved()
{
	playerInventory->PickItem();
}

void AMyPlayerController::InventoryInputRecieved()
{
	playerInventory->ToggleInventoryState();
}