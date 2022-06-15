// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EducationalProject : ModuleRules
{
	public EducationalProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "GameplayTasks",    //Add AI in game to avoid linker error LNK2001
            "NavigationSystem",  //Add AI in game
            "Niagara"
            });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[] 
		{
			"EducationalProject/Public/Player",
			"EducationalProject/Public/Weapon",
            "EducationalProject/Public/Weapon/Components",
            "EducationalProject/Public/Components",
            "EducationalProject/Public/Pickups",
            "EducationalProject/Public/DamageFactors",
            "EducationalProject/Public/UI",
            "EducationalProject/Public/AI",
            "EducationalProject/Public/AI/Services",
            "EducationalProject/Public/Animation"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
