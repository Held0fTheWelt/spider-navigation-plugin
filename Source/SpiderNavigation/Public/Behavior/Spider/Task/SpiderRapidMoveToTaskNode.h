// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SpiderRapidMoveToTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class SPIDERNAVIGATION_API USpiderRapidMoveToTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USpiderRapidMoveToTaskNode();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check", meta = (ExposeOnSpawn = true))
	FBlackboardKeySelector TargetToFollow;
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
protected:
	mutable class AActor* AIController;
};
