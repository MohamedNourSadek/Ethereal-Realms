// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InGameUIManager.generated.h"

UCLASS()
class ETHEREAL_REALMS_API AInGameUIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AInGameUIManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
