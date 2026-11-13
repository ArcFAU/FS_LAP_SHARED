using UnrealBuildTool;
using System.Collections.Generic;

public class FSLapUEEditorTarget : TargetRules
{
	public FSLapUEEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("FSLapUE");
	}
}
