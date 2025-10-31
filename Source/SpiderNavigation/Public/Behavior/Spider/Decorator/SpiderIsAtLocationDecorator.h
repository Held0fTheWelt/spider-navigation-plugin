// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
//#include "Interfaces/Spider/AI/TrackingSpiderAIInterface.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "SpiderIsAtLocationDecorator.generated.h"

/**
 * 
 */
UCLASS()
class SPIDERNAVIGATION_API USpiderIsAtLocationDecorator : public UBTDecorator_BlueprintBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition", meta = (ExposeOnSpawn = true))
	FBlackboardKeySelector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition", meta = (ExposeOnSpawn = true))
	float AcceptableDistance = 50.f;
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual bool CalculateRawConditionValue(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
};
