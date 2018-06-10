// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class MKUE4UtilitiesProjEditorTarget : TargetRules
{
	public MKUE4UtilitiesProjEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.AddRange( new string[] { "MKUE4UtilitiesProj" } );
	}
}
