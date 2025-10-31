// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Interfaces/SpiderNavigationInterface.h"
#include "SpiderCloseEnoughDecorator.generated.h"

/**
 * 
 */
UCLASS()
class SPIDERNAVIGATION_API USpiderCloseEnoughDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	USpiderCloseEnoughDecorator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition", meta = (ExposeOnSpawn = true))
	FBlackboardKeySelector TargetToFollow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition", meta = (ExposeOnSpawn = true))
	float AcceptableDistance;

private:
	mutable class UBlackboardComponent* BB;
	mutable class UObject* Navigation;
	mutable class AActor* Pawn;
	mutable class AActor* Target;

protected:
	virtual bool CalculateRawConditionValue(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

private:
	bool CheckReferences(class UBehaviorTreeComponent& OwnerComp)const;
	bool CheckSpiderCloseEnough() const;
	virtual FString GetStaticDescription() const override;
};
