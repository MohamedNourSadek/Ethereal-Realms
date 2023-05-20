// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "Kismet/GameplayStatics.h"

void UInventory::SetPlayerEnable(bool state)
{
	if(myPlayer != nullptr)
	{
		myPlayer->RecieveInput = state;
	}
	else
	{
		myPlayer = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		SetPlayerEnable(state);
	}
}
