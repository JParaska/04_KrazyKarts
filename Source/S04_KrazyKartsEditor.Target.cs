// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class S04_KrazyKartsEditorTarget : TargetRules
{
	public S04_KrazyKartsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("S04_KrazyKarts");
	}
}
