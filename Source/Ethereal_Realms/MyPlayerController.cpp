// Fill out your copyright notice in the Description page of Project Settings.



#include "MyPlayerController.h"
#include "Ethereal_RealmsGameModeBase.h"
#include "Inventory.h"
#include "Kismet/KismetSystemLibrary.h"

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

	auto nearestObject = GetNearestObject();
/*
	if(nearestObject != nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Detected a near Item"))
	else
		UE_LOG(LogTemp, Display, TEXT("Nothing near you"));
*/
	
}
void AMyPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
		{
			Cast<AEthereal_RealmsGameModeBase>(GetWorld()->GetAuthGameMode())->myInventory->SetPickUIState(true);
			return Cast<APickableItem>(element.GetActor());
		}
	}

	Cast<AEthereal_RealmsGameModeBase>(GetWorld()->GetAuthGameMode())->myInventory->SetPickUIState(false);
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
int AMyPlayerController::RemoveItemFromInventory(UInventoryItemData* item)
{
	if(inventoryItems.Contains(item) == true)
		inventoryItems.Remove(item);

	delete item;

	return inventoryItems.Num();
}