using UnrealBuildTool;

public class FSLapUE : ModuleRules
{
	public FSLapUE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HoMoLapPhysics" });

		// RHI, RenderCore, AudioMixer: only the tests in Dev\ use them.
		PrivateDependencyModuleNames.AddRange(new string[] { "RHI", "RenderCore", "Slate", "SlateCore", "AudioMixer", "Json" });   // Json: LTS results (FSLapLts)

		// FSLapWheel: steering wheels through DirectInput
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicSystemLibraries.AddRange(new string[] { "dinput8.lib", "dxguid.lib" });
		}
	}
}
