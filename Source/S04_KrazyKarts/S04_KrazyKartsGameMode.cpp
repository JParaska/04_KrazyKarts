// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "S04_KrazyKartsGameMode.h"
#include "S04_KrazyKartsPawn.h"
#include "S04_KrazyKartsHud.h"

AS04_KrazyKartsGameMode::AS04_KrazyKartsGameMode()
{
	DefaultPawnClass = AS04_KrazyKartsPawn::StaticClass();
	HUDClass = AS04_KrazyKartsHud::StaticClass();
}
