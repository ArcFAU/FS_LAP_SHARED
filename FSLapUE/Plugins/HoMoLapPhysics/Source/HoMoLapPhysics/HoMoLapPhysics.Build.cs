using UnrealBuildTool;

public class HoMoLapPhysics : ModuleRules
{
	public HoMoLapPhysics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.Add("Core");
	}
}
