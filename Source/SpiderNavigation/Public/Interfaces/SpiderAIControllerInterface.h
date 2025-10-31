// Copyright Yves Tanas 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Navigation/PathFollowingComponent.h"
#include "SpiderAIControllerInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FHandleMoveCompleted, bool);


// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USpiderAIControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for AI controllers in the SpiderNavigation system.
 * Use this to define common AI behavior such as target acquisition,
 * navigation, and visibility checks.
 */
class SPIDERNAVIGATION_API ISpiderAIControllerInterface
{
	GENERATED_BODY()

public:
	virtual FHandleMoveCompleted& GetHandleMoveCompleted() = 0;

	/** Returns the current target actor that the AI is focused on */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spider|Target")
	AActor* GetTargetActor() const;

	/** Returns the world-space location of this AI or its controlled pawn */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spider|Pawn")
	FVector GetLocation() const;

	/** Determines if this AI should perform visibility checks on its target */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spider|Target")
	bool MustCheckTargetVisibility() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spider|Move")
	void MoveTo(const FVector& Destination);
};
