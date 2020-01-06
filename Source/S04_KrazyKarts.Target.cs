// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class S04_KrazyKartsTarget : TargetRules
{
	public S04_KrazyKartsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("S04_KrazyKarts");
	}
}
