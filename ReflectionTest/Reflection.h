// ----------------------------------------------------------------------------
#ifndef ReflectionH
#define ReflectionH
// ----------------------------------------------------------------------------
#ifdef USE_REFLECTION

	#pragma once
	#include <list>
	#include <vector>
	// ----------------------------------------------------------------------------
	typedef char* String;
	#ifndef NULL 
	#define NULL 0L
	#endif
	// ----------------------------------------------------------------------------

	String sGetStringValue(String _sType,void* _pValue);

	// ----------------------------------------------------------------------------
	class CReflectedBaseClass;

	static class CReflectionSupport
	{
		public:
			static void StartClassDefinition(CReflectedBaseClass* _poDefinedClass)
			{
				if (m_bFirstTime)
				{
					m_bFirstTime = false;
				}
				
				m_oDefinedClassStack.push_back(_poDefinedClass);
				// m_poCurDefinedClass = _poDefinedClass;
			}
			
			static CReflectedBaseClass* poGetDefinedClass()
			{
				return (m_oDefinedClassStack[m_oDefinedClassStack.size()-1]);
				// return (m_oDefinedClassStack[0]);
			}

			static void EndClassDefinition()
			{
				return (m_oDefinedClassStack.pop_back());
			}
			
			static bool bReflectedType(String _sType)
			{
				return ( ! ((strcmp(_sType,"int")==0) || (strcmp(_sType,"String")==0)) );
			}

		private:
			static bool								 m_bFirstTime;
			static std::vector<CReflectedBaseClass*> m_oDefinedClassStack;
	};
	
	// ----------------------------------------------------------------------------
	// Cada objeto de cada clase tiene que tener su propia lista de propiedades
	// puesto que 2 objetos de una misma clase, pueden tener valores distintos en
	// sus propiedades, así como diferentes objetos owner.
	// ----------------------------------------------------------------------------
	class CReflectedBaseClass;
	typedef std::list<CReflectedBaseClass*> CPropertyList;

	class CReflectedPropertyList
	{
		public:

			CReflectedPropertyList()
			{
				m_poList = new CPropertyList;
			}

			void Register(CReflectedBaseClass* _pProperty)
			{
				m_poList->push_back( _pProperty );
			};
	
			// Property accessor
			CPropertyList::iterator begin()
			{
				if (m_poList)
					return( m_poList->begin() );
				/*
				else
					return((CPropertyList::iterator)NULL);
				*/
			};

			CPropertyList::iterator end()
			{
				if (m_poList)
					return( m_poList->end() );
				/*
				else
					return((CPropertyList::iterator)NULL);
				*/
			};

		private:

			CPropertyList*			m_poList;
	};
	// ----------------------------------------------------------------------------
	class CReflectedBaseClass
	{
		public:
			CReflectedBaseClass() : 
			m_poProperties(NULL),
			m_poOwner(NULL)
			{
				
			}
			// Set of functions useful for reflection
			
			// Common Fields: Signature
			const virtual String sScope()      { return("Undefined"); };
			const virtual String sType()       { return("CReflectedBaseClass"); };
			const virtual String sName()       { return("Undefined"); };
			
			const virtual unsigned int uiSize(){ return( sizeof(CReflectedBaseClass) ); };
			
													   // Scope		Type				Name
			const virtual String sSignature()  { return( "Undefined CReflectedBaseClass Undefined" ); };
		 // const virtual String int sSignature()  { return( sScope() + sType() + sName() + sValue() };

			// Variant fields
			const virtual String sParent()     { return("Undefined"); };
			const virtual String sValue()      { return("Undefined"); };
			const virtual String sOwner()      { if (poOwner()==NULL) return("NONE"); else return(poOwner()->sName()); };
	
			// 
			virtual CReflectedBaseClass* poOwner() { return( m_poOwner); };

			virtual CReflectedPropertyList* poProperties()
			{
				return(m_poProperties);
			};

		public:
			// 
			void RegisterProperty(CReflectedBaseClass *_poProperty)
			{
				if (m_poProperties == NULL) m_poProperties = new CReflectedPropertyList;
				m_poProperties->Register(_poProperty);
			};

			void SetOwner(CReflectedBaseClass* _poOwner)
			{
				m_poOwner = _poOwner;
			}
			
		private:
			CReflectedPropertyList* m_poProperties;
			CReflectedBaseClass*	m_poOwner;
	};

	// ----------------------------------------------------------------------------
	// Las clases ocupan 4 bytes más porque mantienen un puntero a
	// la tabla virtual de funciones.
	// 
	// Se crea primero una clase __##NewClass para asegurar que el orden de creación
	// creará primero la lista de propiedades (en el constructor de la clase __##NewClass)
	// y luego insertará las propiedades.
	// ----------------------------------------------------------------------------
	#define BEGIN_CLASS_DECLARATION(NewClass,ParentClass)\
	class __##NewClass : public ParentClass\
	{\
		public:\
			__##NewClass()\
			{\
				CReflectionSupport::StartClassDefinition(this);\
			};\
	};\
	\
	class NewClass : public __##NewClass\
	{\
		public:\
			const virtual String sParent() { return (#ParentClass); };\
			const virtual String sType() { return (#NewClass); };\
			const virtual unsigned int uiGetSize() { return( sizeof(NewClass) ); };

	// ----------------------------------------------------------------------------
	#define END_CLASS_DECLARATION(NewClass)\
		private:\
			class __END##NewClass\
			{\
				public:\
				__END##NewClass()\
				{\
					CReflectionSupport::EndClassDefinition();\
				}\
			};\
			\
			__END##NewClass m___END##NewClass;\
	};\

	
	// static NewClass __s##NewClass

	// Constructor Initialization Through Static Instantiation.
	// Si una clase tiene que ser la primera en crearse, será esta.

	// ----------------------------------------------------------------------------
	#define DECLARE_PROPERTY(Scope,Type,Name)\
	Scope:\
	class PROPERTY_##Name : public CReflectedBaseClass\
	{\
		public:\
			Type m_##Name;\
			\
			PROPERTY_##Name()\
			{\
				SetOwner( CReflectionSupport::poGetDefinedClass() );\
				poOwner()->RegisterProperty(this);\
			};\
			\
			inline Type Get()       { return(m_##Name); };\
			inline void Set(Type _##Name) { m_##Name = _##Name; };\
			\
			const virtual String sScope() { return( #Scope ); };\
			const virtual String sType() { return( #Type ); };\
			const virtual String sName() { return( #Name ); };\
			const virtual String sValue() { return( sGetStringValue(#Type,(void*)&m_##Name) ); };\
			const virtual unsigned int uiSize()      { return( sizeof(Type) ); };\
			virtual CReflectedPropertyList* poProperties()\
			{\
				if (CReflectionSupport::bReflectedType(#Type))\
					return ( ((CReflectedBaseClass*)&m_##Name)->poProperties() );\
				else\
					return(NULL);\
			};\
			\
			inline operator Type()  { return( Get() ); };\
			inline void operator = (const Type& _##Name) { Set(_##Name); };\
	};\
	PROPERTY_##Name Name;


	#define DECLARE_PROPERTY_DEF(Scope,Type,Name,DefaultValue)\
	public:\
	class PROPERTY_##Name : public CReflectedBaseClass\
	{\
		public:\
			Type m_##Name;\
			\
			PROPERTY_##Name()\
			{\
				SetOwner( CReflectionSupport::poGetDefinedClass() );\
				poOwner()->RegisterProperty(this);\
				m_##Name = DefaultValue;\
			};\
			\
			inline Type Get()       { return(m_##Name); };\
			inline void Set(Type _##Name) { m_##Name = _##Name; };\
			\
			const virtual String sScope() { return( #Scope ); };\
			const virtual String sType() { return( #Type ); };\
			const virtual String sName() { return( #Name ); };\
			const virtual String sValue() { return( sGetStringValue(#Type,(void*)&m_##Name) ); };\
			const virtual unsigned int uiSize()      { return( sizeof(Type) ); };\
			virtual CReflectedPropertyList* poProperties()\
			{\
				if (CReflectionSupport::bReflectedType(#Type))\
					return ( ((CReflectedBaseClass*)&m_##Name)->poProperties() );\
				else\
					return(NULL);\
			};\
			\
			inline operator Type()  { return( Get() ); };\
			inline void operator = (const Type& _##Name) { Set(_##Name); };\
	};\
	PROPERTY_##Name Name;
// ----------------------------------------------------------------------------
#else // USE_REFLECTION
// ----------------------------------------------------------------------------

	class CReflectedBaseClass
	{
	};

	#define BEGIN_CLASS_DECLARATION(NewClass,ParentClass)\
	class NewClass : public ParentClass\
	{\

	#define END_CLASS_DECLARATION(NewClass)\
	};\

	#define DECLARE_PROPERTY_DEF(Scope,Type,Name,DefaultValue)\
	Scope:\
		Type m_##Name;\

	#define DECLARE_PROPERTY(Scope,Type,Name)\
	Scope:\
		Type m_##Name;\

// ----------------------------------------------------------------------------
#endif // USE_REFLECTION
// ----------------------------------------------------------------------------
#endif // ReflectionH
// ----------------------------------------------------------------------------
