#include "Structs/SpiderDebugRenderer.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

void FSpiderDebugRenderer::EnqueueLine(const FVector& Start, const FVector& End, const FColor& Color, float LifeTime)
{
    FSpiderDebugDrawCommand Cmd;
    Cmd.Type = FSpiderDebugDrawCommand::EType::Line;
    Cmd.A = Start;
    Cmd.B = End;
    Cmd.Color = Color;
    Cmd.LifeTime = LifeTime;
    Pending.Enqueue(MoveTemp(Cmd));
}

void FSpiderDebugRenderer::EnqueueSphere(const FVector& Center, float Radius, const FColor& Color, float LifeTime)
{
    FSpiderDebugDrawCommand Cmd;
    Cmd.Type = FSpiderDebugDrawCommand::EType::Sphere;
    Cmd.A = Center;
    Cmd.Size = Radius;
    Cmd.Color = Color;
    Cmd.LifeTime = LifeTime;
    Pending.Enqueue(MoveTemp(Cmd));
}

void FSpiderDebugRenderer::EnqueuePoint(const FVector& Pos, float Size, const FColor& Color, float LifeTime)
{
    FSpiderDebugDrawCommand Cmd;
    Cmd.Type = FSpiderDebugDrawCommand::EType::Point;
    Cmd.A = Pos;
    Cmd.Size = Size;
    Cmd.Color = Color;
    Cmd.LifeTime = LifeTime;
    Pending.Enqueue(MoveTemp(Cmd));
}

void FSpiderDebugRenderer::FlushToWorld(UWorld* World)
{
    if (!World)
        return;

    FSpiderDebugDrawCommand Cmd;
    while (Pending.Dequeue(Cmd))
    {
        switch (Cmd.Type)
        {
        case FSpiderDebugDrawCommand::EType::Line:
            DrawDebugLine(World, Cmd.A, Cmd.B, Cmd.Color, false, Cmd.LifeTime, 0, 1.f);
            break;

        case FSpiderDebugDrawCommand::EType::Sphere:
            DrawDebugSphere(World, Cmd.A, Cmd.Size, 8, Cmd.Color, false, Cmd.LifeTime);
            break;

        case FSpiderDebugDrawCommand::EType::Point:
            DrawDebugPoint(World, Cmd.A, Cmd.Size, Cmd.Color, false, Cmd.LifeTime);
            break;
        }
    }
}
