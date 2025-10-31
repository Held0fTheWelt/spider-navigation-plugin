// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior/Spider/Task/SpiderRapidMoveToTaskNode.h"


#include "Behavior/BehaviorTreeHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Interfaces/SpiderAIControllerInterface.h"

#include "AIController.h"

USpiderRapidMoveToTaskNode::USpiderRapidMoveToTaskNode()
{

}

void USpiderRapidMoveToTaskNode::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);
    TargetToFollow.ResolveSelectedKey(*GetBlackboardAsset());
}

EBTNodeResult::Type USpiderRapidMoveToTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (AIController == nullptr)
    {
        AIController = Cast<AActor>(OwnerComp.GetAIOwner());
    }

    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }
    const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB)
    {
        return EBTNodeResult::Failed;
    }

    AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetToFollow.SelectedKeyName));
    if (!Target)
    {
        return EBTNodeResult::Failed;
    }

    if (!AIController->GetClass()->ImplementsInterface(USpiderAIControllerInterface::StaticClass()))
    {
        return EBTNodeResult::Failed;
    }

    ISpiderAIControllerInterface::Execute_MoveTo(AIController, Target->GetActorLocation());
    return EBTNodeResult::Succeeded;
}

FString USpiderRapidMoveToTaskNode::GetStaticDescription() const
{
    const FString KeyName = TargetToFollow.SelectedKeyName.IsNone()
        ? TEXT("<none>")
        : TargetToFollow.SelectedKeyName.ToString();

    return FString::Printf(TEXT("TargetToFollow: %s"), *KeyName);
}
