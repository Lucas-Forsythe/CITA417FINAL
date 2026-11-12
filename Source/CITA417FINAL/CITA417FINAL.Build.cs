// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CITA417FINAL : ModuleRules
{
	public CITA417FINAL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"CITA417FINAL",
			"CITA417FINAL/Variant_Platforming",
			"CITA417FINAL/Variant_Platforming/Animation",
			"CITA417FINAL/Variant_Combat",
			"CITA417FINAL/Variant_Combat/AI",
			"CITA417FINAL/Variant_Combat/Animation",
			"CITA417FINAL/Variant_Combat/Gameplay",
			"CITA417FINAL/Variant_Combat/Interfaces",
			"CITA417FINAL/Variant_Combat/UI",
			"CITA417FINAL/Variant_SideScrolling",
			"CITA417FINAL/Variant_SideScrolling/AI",
			"CITA417FINAL/Variant_SideScrolling/Gameplay",
			"CITA417FINAL/Variant_SideScrolling/Interfaces",
			"CITA417FINAL/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
