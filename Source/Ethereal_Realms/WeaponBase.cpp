// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	weaponData = NewObject<UWeaponData>();

	UE_LOG(LogTemp, Display, TEXT("Base Weapon"));
}
