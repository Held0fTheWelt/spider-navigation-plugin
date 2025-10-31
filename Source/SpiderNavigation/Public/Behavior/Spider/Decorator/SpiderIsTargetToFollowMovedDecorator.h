// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SpiderIsTargetToFollowMovedDecorator.generated.h"

/**
 * 
 */
UCLASS()
class SPIDERNAVIGATION_API USpiderIsTargetToFollowMovedDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	USpiderIsTargetToFollowMovedDecorator();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	FBlackboardKeySelector TargetToFollow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	FBlackboardKeySelector TargetLocation;
};
