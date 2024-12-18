// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameSubtitles : ModuleRules
{
	public GameSubtitles(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
				"Overlay",
				"UMG",
				"MediaAssets",
				"MediaUtils",
				"GameplayTags"
			}
		);

		PublicIncludePathModuleNames.AddRange(
			new[]
			{
				"UMG"
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
				// ... add private dependencies that you statically link with here ...
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