// ReflectionTest.cpp : Defines the entry point for the console application.
//
/*
 #include "Reflection.hpp"

   class Caca2{
   public:
    CLASS(Caca2, NullClass);

    PROPERTY(int, MiPropiedad);
    PROPERTY(char*,OtraCosa);

   private:
    FIELD(public, int, m_MiPropiedad);
    FIELD(public, int, m_OtraCosa);

    int get_MiPropiedad() const {
        return m_MiPropiedad;
    }

    void set_MiPropiedad(int v) {
        m_MiPropiedad = v;
    }

    int get_OtraCosa() const {
        return m_OtraCosa;
    }

    void set_OtraCosa(int v) {
        m_OtraCosa = v;
    }
   };
 */
#include "Reflection.h"
#include <stdlib.h>
#include <stdio.h>

// ------------------------------------------------------------
/*
   TODO:
   - Cómo diferenciar que la clase ha sido creada al hacer END_CLASS_DECLARATION, de cuando
   ha sido creada en cualquier otro lugar (variable local, global, new, ...)
 */
// ------------------------------------------------------------

BEGIN_CLASS_DECLARATION(ClaseA, CReflectedBaseClass)
	ClaseA();
	void Caca();

	DECLARE_PROPERTY(public, int, prop1);
	DECLARE_PROPERTY_DEF(public, String, prop2,"nada");

END_CLASS_DECLARATION(ClaseA);

ClaseA::ClaseA()
{
	int a = 0;
}

void ClaseA::Caca()
{
}

BEGIN_CLASS_DECLARATION(ClaseB, ClaseA)

	ClaseB();
	DECLARE_PROPERTY(protected, int, prop3);
	DECLARE_PROPERTY_DEF(public, String, prop4,"ldldld");

END_CLASS_DECLARATION(ClaseB);

ClaseB::ClaseB()
{
	int a = 0;
}

BEGIN_CLASS_DECLARATION(ClaseC, CReflectedBaseClass)

	DECLARE_PROPERTY(public, ClaseB, prop5);

END_CLASS_DECLARATION(ClaseTest);


// ------------------------------------------------------------
void dlg_DisplayProperties(CReflectedBaseClass* _poClass,int _iLevel)
{
	char szTabs[100];
	memset(szTabs,'\t',_iLevel);
	szTabs[_iLevel]=0;

	printf("%s%s: %s %s =%s\n",szTabs, _poClass->sScope(),_poClass->sType(),_poClass->sName(),_poClass->sValue());
	if ( _poClass->poProperties() == NULL) return;

	for (CPropertyList::iterator it = _poClass->poProperties()->begin(); it != _poClass->poProperties()->end(); it++)
		dlg_DisplayProperties((*it),_iLevel+1);
}
// ------------------------------------------------------------
int main(void)
{
	ClaseC ott;

	// insert your code here
	dlg_DisplayProperties( &ott,0);
}
// ------------------------------------------------------------
