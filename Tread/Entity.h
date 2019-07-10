// ENTITY.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
////////////////////////////////////////////////////////////////////////////
// Defines the entity objects actually linked into maps.
////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H
#define ENTITY_H

#include "MathLib.h"
#include "LinkedList.h"
#include "MapObject.h"
#include "TExport.h"

// $sb "entity.h block1"

#define ENTITY_STORABLE_OBJECT_ID	0xF0000000

struct ENTITYKEY;
struct _T_EXPORT ENTITYKEY
{
	ENTITYKEY* prev, *next;
	
	ENTITYKEY()
	{
		pKeyDef = NULL;
		sName = sValue = "";
	}

	ENTITYKEY(struct ENTITYKEYDEFINITION* pKeyDefToCopy);

	ENTITYKEY(const ENTITYKEY& KeyToCopy)
	{
		sName = KeyToCopy.sName;
		sValue = KeyToCopy.sValue;
		pKeyDef = KeyToCopy.pKeyDef;
		prev = next = NULL;
	}

	
	struct ENTITYKEYDEFINITION* pKeyDef;	// Definition of this key.
	CString sName, sValue;
};

// Render classes.
class CGDIEntityRenderer;
// End Render Classes.

// Defines an entity.
class _T_EXPORT CEntity : public CObjectGroup, public CBaseTargetObject
{

	friend class CGDIEntityRenderer;

protected:

	CString m_sClassName;
	struct ENTITYDEFINITION* m_pEntDef;
	CLinkedList<struct ENTITYKEY>* m_pKeyList;
	bool WriteKey(ENTITYKEY* pKey, CFile& hFile, int nVersion);
	ENTITYKEY* ReadKey(CFile& hFile, int nVersion);
	int m_nMiscFlags[4];

public:

	CEntity();
	CEntity(const CEntity& EntToCopy);
	virtual ~CEntity();

	virtual bool InitFromDefinition(ENTITYDEFINITION* pEntDef);

	CString GetClassName() const { return m_sClassName; }

	bool DeleteKey(CString sName);
	ENTITYKEY* GetKey(CString sName);
	CString GetKeyValue(CString sName);
	CLinkedList<ENTITYKEY>* GetKeyList() const { return m_pKeyList; }
	ENTITYDEFINITION* GetEntityDefinition() { return m_pEntDef; }
	virtual bool SetClassName(CString sClass);

	virtual bool IsOwner();

	virtual void PreDelete(CTread3DDoc* pDocument);

	virtual CBaseObject* Clone() { return new CEntity(*this); }

	// Changes one entity it an duplicate of the parameter,
	// and maintains original brushes.
	virtual void MorphEntity(CEntity* pEntity);

	// From CBaseGroupableObject
	virtual int GetObjectClass();
	virtual int GetObjectInterfaces();
	virtual void* GetInterface(int nInterface);

	virtual bool AddObject(CBaseGroupableObject* pObject);
	virtual bool RemoveObject(CBaseGroupableObject* pObject);

	// From CBaseTargetObject
	virtual CString GetTargetableName();
	virtual void SetTargetableName(CString sName, CTread3DDoc* pDocument);
	virtual CString GetTargetName();
	virtual void SetTargetName(CString sName, CTread3DDoc* pDocument);

	// From CBaseTransformableObject
	virtual void SetOrigin(vec3_t vOrigin) { VectorCopy(vOrigin, m_vOrigin); }
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin);
	virtual void BuildRange();
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView);

	// From CBaseStorableObject.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);
	virtual int InitAfterLoad(CTread3DDoc* pDocument);
	virtual int PostInitAfterLoad(CTread3DDoc* pDocument, int nCode);
};

// $eb

#endif