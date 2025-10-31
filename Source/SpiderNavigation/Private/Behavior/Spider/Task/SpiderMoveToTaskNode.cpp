// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior/Spider/Task/SpiderMoveToTaskNode.h"

#include "Behavior/BehaviorTreeHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Interfaces/SpiderAIControllerInterface.h"

#include "AIController.h"

USpiderMoveToTaskNode::USpiderMoveToTaskNode()
{
    
}

void USpiderMoveToTaskNode::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);
    TargetLocation.ResolveSelectedKey(*GetBlackboardAsset());
}

EBTNodeResult::Type USpiderMoveToTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Warning, TEXT("USpiderMoveToTaskNode::ExecuteTask - Start"));

	CachedOwnerComp = &OwnerComp;

    if(AIController == nullptr)
    {
        AIController = Cast<AActor>(OwnerComp.GetAIOwner());
		UE_LOG(LogTemp, Warning, TEXT("USpiderMoveToTaskNode::ExecuteTask - AIController gesetzt"));
	}

    if (!AIController)
    {
		UE_LOG(LogTemp, Warning, TEXT("USpiderMoveToTaskNode::ExecuteTask - Kein AIController gefunden"));
        return EBTNodeResult::Failed;
    }
  
    const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB)
    {
		UE_LOG(LogTemp, Warning, TEXT("USpiderMoveToTaskNode::ExecuteTask - Kein BlackboardComponent gefunden"));
        return EBTNodeResult::Failed;        
    }

    const FVector VTargetLocation = BB->GetValueAsVector(TargetLocation.SelectedKeyName);

    if(!AIController->GetClass()->ImplementsInterface(USpiderAIControllerInterface::StaticClass()))
    {
		UE_LOG(LogTemp, Warning, TEXT("USpiderMoveToTaskNode::ExecuteTask - AIController implementiert nicht das Interface"));
        return EBTNodeResult::Failed;
	}
    
    ISpiderAIControllerInterface* SpiderCtrl = Cast<ISpiderAIControllerInterface>(AIController);
    if (!SpiderCtrl)
    {
		UE_LOG(LogTemp, Warning, TEXT("USpiderMoveToTaskNode::ExecuteTask - Konnte SpiderCtrl nicht casten"));
        return EBTNodeResult::Failed;
    }
	UE_LOG(LogTemp, Warning, TEXT("USpiderMoveToTaskNode::ExecuteTask - SpiderCtrl gefunden"));
    // Delegate binden
    SpiderCtrl->GetHandleMoveCompleted().AddUObject(this, &USpiderMoveToTaskNode::HandleMoveCompleted);
    
	UE_LOG(LogTemp, Warning, TEXT("USpiderMoveToTaskNode::ExecuteTask - MoveTo wird gestartet"));
    ISpiderAIControllerInterface::Execute_MoveTo(AIController, VTargetLocation);
	UE_LOG(LogTemp, Warning, TEXT("USpiderMoveToTaskNode::ExecuteTask - MoveTo gestartet"));
    return EBTNodeResult::InProgress;
}

void USpiderMoveToTaskNode::HandleMoveCompleted(bool Success)
{
    if (!AIController || !CachedOwnerComp)
        return;

    if (Success)
    {
        ISpiderAIControllerInterface* SpiderCtrl = Cast<ISpiderAIControllerInterface>(AIController);
        if (SpiderCtrl)
            SpiderCtrl->GetHandleMoveCompleted().RemoveAll(this);
            FinishLatentTask(*CachedOwnerComp, Success ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
    }
}

FString USpiderMoveToTaskNode::GetStaticDescription() const
{
    const FString KeyName = TargetLocation.SelectedKeyName.IsNone()
        ? TEXT("<none>")
        : TargetLocation.SelectedKeyName.ToString();

    return FString::Printf(TEXT("Target: %s"), *KeyName);
}
