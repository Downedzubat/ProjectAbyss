// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectAbyssV2 : ModuleRules
{
	public ProjectAbyssV2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
