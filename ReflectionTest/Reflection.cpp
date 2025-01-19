// ReflectionTest.cpp : Defines the entry point for the console application.
//
#include "Reflection.h"
// -----------------------------------------------------------
#ifdef USE_REFLECTION
// -----------------------------------------------------------

	#include <string.h>
	#include <stdio.h>
	#include <list>
	
	// ReflectionTest.cpp : Defines the entry point for the console application.
	//
	#include "Reflection.h"

	#include <string.h>
	#include <stdio.h>
	#include <list>
	// -----------------------------------------------------------
	bool							  CReflectionSupport::m_bFirstTime = true;
	std::vector<CReflectedBaseClass*> CReflectionSupport::m_oDefinedClassStack;
	// xxx int						CReflectedPropertyList::m_iRefCount = 0;
	// xxx CPropertyList*			CReflectedPropertyList::m_poCurList = NULL;
	// -----------------------------------------------------------
	typedef char* String;

	// ------------------------------------------------------------
	// Estas variables deben estar dentro de un cpp
	// ------------------------------------------------------------
	static char sValueBuff[32];
	static String ssValue = (String)&sValueBuff;

	String sGetStringValue(String _sType,void* _pValue)
	{
		if (! strcmp(_sType,"int"))
		{
			sprintf(ssValue,"%d", *((int*)_pValue)  );
			return(ssValue);
		}
		else if (! strcmp(_sType,"String"))
		{
			if (*(String*)_pValue != NULL)
				return( *(String*)_pValue );
			else
				return( "<NULL>" );
		}
		else
		{
			sprintf(ssValue,"No Interpretable Value");
			return(ssValue);
		}
	}
	// -----------------------------------------------------------

// -----------------------------------------------------------	
#else // USE_REFLECTION
// -----------------------------------------------------------

// -----------------------------------------------------------
#endif // USE_REFLECTION
// -----------------------------------------------------------