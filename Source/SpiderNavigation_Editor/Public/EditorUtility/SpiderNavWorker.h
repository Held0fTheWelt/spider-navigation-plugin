// SpiderNavWorker.h
#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/Event.h"
#include "Containers/Queue.h"

//#include "SpiderNavWorker.generated.h"
class USpiderNavigationBuilderWidget;

/**
 * Hintergrund-Worker für Spider Navigation Tasks
 */
class FSpiderNavWorker : public FRunnable
{
public:
    FSpiderNavWorker(USpiderNavigationBuilderWidget* InOwner);
    virtual ~FSpiderNavWorker() override;

    // FRunnable Interface
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    // Public API
    void EnqueueTask(TFunction<void()> InTask);
    bool IsRunning() const { return bRunning; }

private:
    TQueue<TFunction<void()>, EQueueMode::Mpsc> TaskQueue;
    FEvent* WakeEvent;
    FRunnableThread* Thread;
    FThreadSafeBool bRunning;
    USpiderNavigationBuilderWidget* Owner;
};
