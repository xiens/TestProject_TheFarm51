// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class TestProject : ModuleRules
{

	public TestProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "ProceduralMeshComponent" });

        LoadDelaunayTriangulation(Target);
    }

    private string ModulePath
    {
        get { return ModuleDirectory; }
    }


    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }


    public bool LoadDelaunayTriangulation(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "DelaunayTriangulation", "Libraries");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "DelabellaLib" + ".lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "DelaunayTriangulation", "Includes"));
        }

        Definitions.Add(string.Format("WITH_DELAUNAY_TRIANGULATION_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}
