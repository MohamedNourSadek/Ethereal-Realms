// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyPlayerController::AMyPlayerController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto nearestObject = GetNearestObject();

	if(nearestObject != nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Detected a near Item"))
	else
		UE_LOG(LogTemp, Display, TEXT("Nothing near you"));
}

// Called to bind functionality to input
void AMyPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

APickableItem* AMyPlayerController::GetNearestObject() const
{
	TArray<FHitResult> HitArray;
	FVector location = this->GetActorLocation();
	TArray<AActor*> exceptionActors {GetOwner()};

	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		location,
		location,
		pickRad,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldDynamic),
		false,
		exceptionActors,
		EDrawDebugTrace::ForOneFrame,
		HitArray,
		true);

	for(auto element : HitArray)
	{
		if(element.GetActor()->Tags.Contains("Pickable"))
		{
			return Cast<APickableItem>(element.GetActor());
		}
	}

	return nullptr;
}


