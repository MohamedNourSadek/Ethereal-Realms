// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableItem.generated.h"

UCLASS()
class ETHEREAL_REALMS_API APickableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	APickableItem();
protected:
	virtual void BeginPlay() override;
public:	
	bool isPicked = false;
	virtual void Tick(float DeltaTime) override;
	FString itemName = "Item"; 
};
