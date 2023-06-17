// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class ETHEREAL_REALMS_API ASword : public AWeaponBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
