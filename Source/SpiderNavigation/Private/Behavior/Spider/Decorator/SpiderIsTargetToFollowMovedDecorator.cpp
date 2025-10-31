// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior/Spider/Decorator/SpiderIsTargetToFollowMovedDecorator.h"

#include "Behavior/BehaviorTreeHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

USpiderIsTargetToFollowMovedDecorator::USpiderIsTargetToFollowMovedDecorator()
{
	NodeName = TEXT("Spider Is TargetToFollow Moved");
	FlowAbortMode = EBTFlowAbortMode::Both;

	TargetToFollow.AddObjectFilter(this,
		GET_MEMBER_NAME_CHECKED(USpiderIsTargetToFollowMovedDecorator, TargetToFollow),
		UObject::StaticClass());

	TargetLocation.AddVectorFilter(this,
		GET_MEMBER_NAME_CHECKED(USpiderIsTargetToFollowMovedDecorator, TargetLocation));
}
void USpiderIsTargetToFollowMovedDecorator::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	TargetToFollow.ResolveSelectedKey(*GetBlackboardAsset());
	TargetLocation.ResolveSelectedKey(*GetBlackboardAsset());
}

bool USpiderIsTargetToFollowMovedDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
		return false;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetToFollow.SelectedKeyName));
	if (!Target)
		return false;

	const FVector LastKnownLocation = BB->GetValueAsVector(TargetLocation.SelectedKeyName);
	const FVector CurrentLocation = Target->GetActorLocation();

	const bool bHasMoved = !CurrentLocation.Equals(LastKnownLocation, 5.0f); // 5 cm Toleranz
	return bHasMoved;
}

FString USpiderIsTargetToFollowMovedDecorator::GetStaticDescription() const
{
	const FString AbortText = UBehaviorTreeHelpers::AbortModeToText(FlowAbortMode);
	const FString AbortLine = AbortText.IsEmpty() ? TEXT("") : (AbortText + TEXT("\n"));

	return FString::Printf(TEXT("%sTargetToFollow: %s\nTargetLocation: %s"),
		*AbortLine,
		*TargetToFollow.SelectedKeyName.ToString(),
		*TargetLocation.SelectedKeyName.ToString());
}

//if (bHasMoved)
//{
//	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocation.SelectedKeyName, CurrentLocation);
//}