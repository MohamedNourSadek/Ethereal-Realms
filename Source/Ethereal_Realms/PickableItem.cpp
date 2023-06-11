// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"

// Sets default values
APickableItem::APickableItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APickableItem::BeginPlay()
{
	Super::BeginPlay();
}

void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


