#include "IBluLoader.h"
#include "Interfaces/IPluginManager.h"
#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include <string>

#if PLATFORM_WINDOWS
#include "Windows/WindowsPlatformProcess.h"
#endif

class FBluLoader : public IBluLoader
{

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		FString LibPath = FPaths::ConvertRelativePathToFull(IPluginManager::Get().FindPlugin("BLUI")->GetBaseDir() + "/ThirdParty/cef/");

		// If we're on Windows we need to load DLLs from our custom path
		#if PLATFORM_WINDOWS
			LibPath += "Win/shipping/";
			FPlatformProcess::PushDllDirectory(*LibPath);
            UE_LOG(LogBluLoader, Log, TEXT("patched dll directory paths"));
		#endif
        
        #if PLATFORM_MAC
            // We need to load OUR CEF3 framework bundle here. It uses this identifier: org.chromium.ContentShell.BLUI.framework
            LibPath += "Mac/lib/Chromium Embedded Framework.framework/Chromium Embedded Framework";
            void* framework_hdl = dlopen(TCHAR_TO_ANSI(*LibPath), RTLD_NOW);
            UE_LOG(LogBluLoader, Log, TEXT("dlopen has loaded CEF framework"));
        #endif

		UE_LOG(LogBluLoader, Log, TEXT("STATUS: BLUI Ready to Load!"));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogBluLoader, Log, TEXT("STATUS: BLUI Has Shutdown"));
	}

};

IMPLEMENT_MODULE( FBluLoader, BluLoader )
DEFINE_LOG_CATEGORY(LogBluLoader);