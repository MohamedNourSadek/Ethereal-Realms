#include "Sword.h"

void ASword::BeginPlay()
{
	Super::BeginPlay();

	weaponData->baseDamage = 50;
	weaponData->baseSpeed = 50;
	itemName = "Sword";
}
