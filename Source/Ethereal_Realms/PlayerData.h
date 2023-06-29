// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerData.generated.h"

/**
 * 
 */
USTRUCT()
struct ETHEREAL_REALMS_API FPlayerData 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) int power = 0;
	UPROPERTY(EditAnywhere) int swordsmanship = 0;
	UPROPERTY(EditAnywhere) int tactics = 0;
};
