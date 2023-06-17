// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class ETHEREAL_REALMS_API UWeaponData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) int baseDamage;
	UPROPERTY(EditAnywhere) int baseSpeed;
};
