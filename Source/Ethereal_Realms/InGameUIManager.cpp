// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUIManager.h"

#include "Inventory.h"

extern UInventory* playerInventory;

AInGameUIManager::AInGameUIManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInGameUIManager::BeginPlay()
{
	Super::BeginPlay();
}

void AInGameUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

