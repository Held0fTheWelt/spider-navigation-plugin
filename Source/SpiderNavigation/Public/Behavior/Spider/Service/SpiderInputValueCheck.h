// Copyright Yves Tanas 2025

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "SpiderInputValueCheck.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SPIDERNAVIGATION_API USpiderInputValueCheck : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:
	USpiderInputValueCheck();

protected:
	virtual void TickNode(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	/** Notify called after GameplayTask changes state from Active (finishing or pausing) */
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	FBlackboardKeySelector TargetToFollow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	FBlackboardKeySelector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	float SenseRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	TArray<TEnumAsByte<EObjectTypeQuery>> DesiredObjectTypes;

private:
	class UObject* Navigation;
	class AActor* AIController;
	class AActor* Target;
	FVector CurrentLocation;

private:
	bool CheckReferences(class UBehaviorTreeComponent& OwnerComp);
	void PerformValueCheck(class UBehaviorTreeComponent& OwnerComp);
};
