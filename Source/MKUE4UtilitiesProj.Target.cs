// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class MKUE4UtilitiesProjTarget : TargetRules
{
	public MKUE4UtilitiesProjTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.AddRange( new string[] { "MKUE4UtilitiesProj" } );
	}
}
