// Copyright Yves Tanas 2025

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "SpiderNavGridEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SPIDERNAVIGATION_EDITOR_API USpiderNavGridEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool SaveGrid(FString SaveSlotName, int32 UserIndex, class USpiderNavGridSaveGame* SaveGame);
};
