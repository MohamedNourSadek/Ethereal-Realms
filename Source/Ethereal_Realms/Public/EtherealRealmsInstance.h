// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EtherealRealmsInstance.generated.h"

/**
 * 
 */
UCLASS()
class ETHEREAL_REALMS_API UEtherealRealmsInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;


	UFUNCTION(Exec)
	void HostGame();
};
