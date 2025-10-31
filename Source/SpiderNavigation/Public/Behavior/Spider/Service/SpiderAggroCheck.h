// @Yves Tanas 2020

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "SpiderAggroCheck.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SPIDERNAVIGATION_API USpiderAggroCheck : public UBTService_BlueprintBase
{
	GENERATED_BODY()
	
public:
	USpiderAggroCheck();

protected:
	virtual void TickNode(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	/** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	/** Notify called after GameplayTask changes state from Active (finishing or pausing) */
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	FVector CurrentLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	FBlackboardKeySelector TargetToFollow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	FBlackboardKeySelector TargetLocation;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Value Check")
	//TArray<EObjectTypeQuery> DesiredObjectTypes;
	float AggroRadius;

private:
	class USpiderNavigationInterface* Navigation;
	class USpiderAIControllerInterface* AIController;
	class AActor* Target;
//
//private:
//	bool CheckReferences();
//	void PerformAggroCheck();
};
