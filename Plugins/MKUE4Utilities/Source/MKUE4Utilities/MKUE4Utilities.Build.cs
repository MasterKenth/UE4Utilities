// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MKUE4Utilities : ModuleRules
{
	public MKUE4Utilities(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"MKUE4Utilities/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"MKUE4Utilities/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "Paper2D",
                "Slate",
                "SlateCore"
                //"Kismet",
                //"InputCore",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
            {
                "Paper2D",
                //"Core",
                //"CoreUObject",
				//"Engine",
                //"UMG",
                //"Kismet",
                //"InputCore",
                "Slate",
				"SlateCore",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
