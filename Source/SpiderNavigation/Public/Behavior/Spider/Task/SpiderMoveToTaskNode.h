// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "SpiderMoveToTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class SPIDERNAVIGATION_API USpiderMoveToTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USpiderMoveToTaskNode();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check", meta = (ExposeOnSpawn = true))
	FBlackboardKeySelector TargetLocation;
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
protected:
	class AActor* AIController;
	class UBehaviorTreeComponent* CachedOwnerComp;

private:
	void HandleMoveCompleted(bool Success);
};
