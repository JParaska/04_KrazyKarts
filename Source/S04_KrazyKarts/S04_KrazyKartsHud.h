// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "S04_KrazyKartsHud.generated.h"


UCLASS(config = Game)
class AS04_KrazyKartsHud : public AHUD
{
	GENERATED_BODY()

public:
	AS04_KrazyKartsHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
