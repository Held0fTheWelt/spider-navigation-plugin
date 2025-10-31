// Copyright Yves Tanas 2025


#include "Actors/SpiderNavGridBuilderVolume.h"

#include "Components/BoxComponent.h"

ASpiderNavGridBuilderVolume::ASpiderNavGridBuilderVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	VolumeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("VolumeBox"));
	VolumeBox->BodyInstance.SetCollisionProfileName("Custom");
	VolumeBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	VolumeBox->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VolumeBox->SetWorldScale3D(FVector(50.f, 50.f, 50.f));
	RootComponent = VolumeBox;
}
