#pragma once

#include "SpiderNavNode.generated.h"

/** Describes navigation point in grid */
USTRUCT()
struct FSpiderNavNode
{
	GENERATED_BODY()

	/** Location of node */
	UPROPERTY()
	FVector Location;

	/** Normal of node from nearest world object with collision */
	UPROPERTY()
	FVector Normal;

	/** Index (id) of node */
	UPROPERTY()
	int32 Index;

	/** Relations */
	TArray <FSpiderNavNode*> Neighbors;

	/** F-value of node from A-star */
	float F;

	/** G-value of node from A-star */
	float G;

	/** H-value of node from A-star */
	float H;

	/** Opened property of node from A-star */
	bool Opened;

	/** Closed propery of node from A-star */
	bool Closed;

	/** Index (id) of parent node from A-star */
	int32 ParentIndex;

	/** Initialization of node */
	FSpiderNavNode()
	{
		Location = FVector(0.0f, 0.0f, 0.0f);
		Index = -1;
		F = 0.0f;
		G = 0.0f;
		H = 0.0f;
		Opened = false;
		Closed = false;
		ParentIndex = -1;

		Neighbors.Empty();
	}

	/** Resets metriks of A-star */
	void ResetMetrics()
	{
		F = 0.0f;
		G = 0.0f;
		H = 0.0f;
		Opened = false;
		Closed = false;
		ParentIndex = -1;
	}
};

/** Comparator < of nodes by F-value */
struct LessThanByNodeF {
	bool operator()(const FSpiderNavNode* lhs, const FSpiderNavNode* rhs) const {
		return lhs->F > rhs->F;
	}
};

/**  Comparator < of nodes by H-value */
struct LessThanByNodeH {
	bool operator()(const FSpiderNavNode* lhs, const FSpiderNavNode* rhs) const {
		return lhs->H > rhs->H;
	}
};