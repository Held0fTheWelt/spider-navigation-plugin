// Copyright Yves Tanas 2025


#include "Subsystems/SpiderNavGridEditorSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "SaveGame/SpiderNavGridSaveGame.h"


bool USpiderNavGridEditorSubsystem::SaveGrid(FString SaveSlotName, int32 UserIndex, USpiderNavGridSaveGame* SaveGame)
{
	return UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName, UserIndex);
}