// SpiderNavWorker.cpp
#include "EditorUtility/SpiderNavWorker.h"
#include "EditorUtility/SpiderNavigationBuilderWidget.h"
#include "HAL/PlatformProcess.h"
#include "Async/Async.h"

FSpiderNavWorker::FSpiderNavWorker(USpiderNavigationBuilderWidget* InOwner)
    : WakeEvent(FPlatformProcess::GetSynchEventFromPool(false))
    , Thread(nullptr)
    , bRunning(true)
    , Owner(InOwner)
{
    Thread = FRunnableThread::Create(this, TEXT("SpiderNavWorker"), 0, TPri_BelowNormal);
    UE_LOG(LogTemp, Log, TEXT("[SpiderWorker] Thread created."));
}

FSpiderNavWorker::~FSpiderNavWorker()
{
    Stop();
    if (Thread)
    {
        Thread->Kill(true);
        delete Thread;
        Thread = nullptr;
    }
    FPlatformProcess::ReturnSynchEventToPool(WakeEvent);
    WakeEvent = nullptr;

    UE_LOG(LogTemp, Log, TEXT("[SpiderWorker] Thread destroyed."));
}

bool FSpiderNavWorker::Init()
{
    UE_LOG(LogTemp, Log, TEXT("[SpiderWorker] Init OK."));
    return true;
}

uint32 FSpiderNavWorker::Run()
{
    while (bRunning)
    {
        TFunction<void()> Task;
        if (TaskQueue.Dequeue(Task))
        {
            // Führe Task aus (Worker-Kontext)
            Task();
        }
        else
        {
            // Keine Tasks → warten, um CPU zu schonen
            WakeEvent->Wait(100); // 100ms oder bis Signal
        }
    }
    return 0;
}

void FSpiderNavWorker::Stop()
{
    bRunning = false;
    if (WakeEvent)
        WakeEvent->Trigger();
}

void FSpiderNavWorker::Exit()
{
    UE_LOG(LogTemp, Log, TEXT("[SpiderWorker] Exiting."));
}

void FSpiderNavWorker::EnqueueTask(TFunction<void()> InTask)
{
    TaskQueue.Enqueue(MoveTemp(InTask));
    WakeEvent->Trigger();
}
