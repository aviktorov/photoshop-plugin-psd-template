#define vendorName			"Fate Studio"
#define plugInAETEComment 	"Template automation plug-in"

#define plugInSuiteID		'sdKG'
#define plugInClassID		plugInSuiteID
#define plugInEventID		'lstN'

//-------------------------------------------------------------------------------
//	Set up included files for Macintosh and Windows.
//-------------------------------------------------------------------------------

#include "PIDefines.h"

#ifdef __PIMac__
	#include "TemplateAutomationTerminology.h"
	#include "PIGeneral.r"
	#include "PIUtilities.r"
#elif defined(__PIWin__)
	#include "TemplateAutomationTerminology.h"
	#define Rez
	#include "PIGeneral.h"
	#include "PIUtilities.r"
#endif

#include "PITerminology.h"
#include "PIActions.h"

resource 'PiPL' (TemplateAutomationResourceID,plugInName " PiPL",purgeable) {
	{
		Kind { Actions },
		Name { plugInName "..." },
		Category { vendorName },
		Version { (latestActionsPlugInVersion << 16) | latestActionsPlugInSubVersion },

		#ifdef __PIMac__
			#if (defined(__x86_64__))
				CodeMacIntel64 { "AutoPluginMain" },
			#endif
			#if (defined(__i386__))
				CodeMacIntel32 { "AutoPluginMain" },
			#endif
		#else
			#if defined(_WIN64)
				CodeWin64X86 { "AutoPluginMain" },
			#else
				CodeWin32X86 { "AutoPluginMain" },
			#endif
		#endif

		HasTerminology { 
			plugInClassID, 
			plugInEventID, 
			TemplateAutomationResourceID, 
			vendorName " " plugInName	// Unique string.
		},
		
		Persistent {},
		
		Messages {
			startupRequired,
			doesNotPurgeCache,
			shutdownRequired,
			acceptProperty
		},
	}
};

resource 'aete' (TemplateAutomationResourceID,plugInName " dictionary",purgeable) {
	1, 0, english, roman,
	{
		vendorName,
		"Game development utilities",
		plugInSuiteID,
		1,
		1,
		{
			plugInName,
			plugInAETEComment,
			plugInClassID,
			plugInEventID,

			NO_REPLY,
			IMAGE_DIRECT_PARAMETER,
			{
				"action set",
				keyPIActionSet,
				typeChar,
				"",
				flagsSingleParameter,

				"action",
				keyPIAction,
				typeChar,
				"",
				flagsSingleParameter
			}
		},
		{},
		{},
		{}
	}
};

resource 'STR ' (TemplateAutomationAboutID,plugInName " About Text",purgeable) {
	plugInName "\n\n"
	"Version " VersionString " "
	"Release " ReleaseString "\n"
	"Copyright " plugInCopyrightYear "-" CurrentYear ", Fate Studio.\n"
	"All Rights Reserved.\n\n"
	plugInDescription
};
