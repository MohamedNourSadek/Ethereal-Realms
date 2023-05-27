// Fill out your copyright notice in the Description page of Project Settings.


#include "Ethereal_RealmsGameModeBase.h"

void AEthereal_RealmsGameModeBase::SetInventory(UInventory* _myInventory)
{
	myInventory= _myInventory;
	UE_LOG(LogTemp, Error, TEXT("%s"), *myInventory->GetName());
}