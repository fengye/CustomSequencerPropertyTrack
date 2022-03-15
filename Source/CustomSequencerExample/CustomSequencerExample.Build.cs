// Some copyright should be here...

using UnrealBuildTool;

public class CustomSequencerExample : ModuleRules
{
	public CustomSequencerExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				"CustomSequencerExample/Public"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				"CustomSequencerExample/Private"
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"InputCore",
				"EditorStyle",
				"Slate",
				"SlateCore",
				"MovieScene",
				"MovieSceneTracks",
				"MovieSceneTools",
				"Sequencer",
				"TimeManagement",
				"UnrealEd",
			}
		);
	}
}
