// ENTITY.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "LinkedList.h"
#include "Entity.h"
#include "GameAPI.h"
#include "MapObject.h"
#include "RenderAPI.h"
#include "Geometry.h"
#include "futils.h"

ENTITYKEY::ENTITYKEY(struct ENTITYKEYDEFINITION* pKeyDefToCopy)
{
	sName = pKeyDefToCopy->sName;
	sValue = pKeyDefToCopy->sValue;
	pKeyDef = pKeyDefToCopy;
	prev = next = NULL;
}

// Constructors / Destructors.
CEntity::CEntity() : CObjectGroup()
{
	m_sClassName = "";
	m_pEntDef = NULL;
	m_pKeyList = new CLinkedList<ENTITYKEY>();
	m_nInterfaceRenderFlags = RF_OWNED;	// All subs are drawn as owned.
	memset(m_nMiscFlags, -1, sizeof(m_nMiscFlags));
}

CEntity::~CEntity()
{
	if(m_pKeyList != NULL)
		delete m_pKeyList;
	m_pKeyList = NULL;
}

bool CEntity::DeleteKey(CString sName)
{
	ENTITYKEY* pKey = GetKey(sName);
	if(pKey == NULL)
		return false;

	m_pKeyList->DeleteItem(pKey);
	return true;
}

void CEntity::PreDelete(CTread3DDoc* pDocument)
{
	pDocument->GetTargetAPI()->SetTargetBinding("", this);
}

CEntity::CEntity(const CEntity& EntToCopy) : CObjectGroup(EntToCopy)
{
	m_sClassName = EntToCopy.m_sClassName;
	m_pEntDef = EntToCopy.m_pEntDef;
	m_pKeyList = new CLinkedList<ENTITYKEY>(*EntToCopy.m_pKeyList);
	memcpy(m_nMiscFlags, EntToCopy.m_nMiscFlags, sizeof(m_nMiscFlags));
	m_nInterfaceRenderFlags = RF_OWNED;
}

CString CEntity::GetTargetableName()
{
	return GetKeyValue(GetGameAPI()->GetCurrentGame()->GetNameKeyString());
}

void CEntity::SetTargetableName(CString sName, CTread3DDoc* pDocument)
{
	ENTITYKEY* pKey = GetKey(GetGameAPI()->GetCurrentGame()->GetNameKeyString());
	if(pKey == NULL)
		return;
	pKey->sValue = sName;
}

CString CEntity::GetTargetName()
{
	return GetKeyValue(GetGameAPI()->GetCurrentGame()->GetTargetKeyString());
}

void CEntity::SetTargetName(CString sName, CTread3DDoc* pDocument)
{
	ENTITYKEY* pKey = GetKey(GetGameAPI()->GetCurrentGame()->GetTargetKeyString());
	if(pKey == NULL)
		return;
	pKey->sValue = sName;
}

void CEntity::Reflect(int nAxis, vec3_t vFlipOrigin)
{
	if(m_pObjectList->IsEmpty() == false)
	{
		GDATA gd;
		gd.m_pList = m_pObjectList;
		VectorCopy(vFlipOrigin, gd.m_vOrigin);
		G_Reflect(nAxis, &gd);
		BuildRange();
	}
	else
		m_vOrigin[nAxis] = vFlipOrigin[nAxis] + (vFlipOrigin[nAxis] - m_vOrigin[nAxis]);
}

bool CEntity::AddObject(CBaseGroupableObject* pObject)
{
	CObjectGroup::AddObject(pObject);
	return true;
}

bool CEntity::RemoveObject(CBaseGroupableObject* pObject)
{
	CObjectGroup::RemoveObject(pObject);
	return true;
}

// Set's the entity's class name, and the entity def.
bool CEntity::SetClassName(CString sClass)
{
	ENTITYDEFINITION* pDef = GetGameAPI()->GetCurrentGame()->FindEntityDefinition(sClass);
	if(pDef == NULL)
		return false;

	this->m_sClassName = sClass;
	this->m_pEntDef = pDef;

	// Attach all keys to their defs.
	ENTITYKEY* pKey;
	ENTITYKEYDEFINITION* pKDef;

	for(pKDef = pDef->pKeyList->ResetPos(); pKDef != NULL; pKDef = pDef->pKeyList->GetNextItem())
	{
		pKey = this->GetKey(pKDef->sName);
		if(pKey != NULL)
			pKey->pKeyDef = pKDef;
	}

	return true;
}

// Simply copy the keys.
void CEntity::MorphEntity(CEntity* pEntity)
{
	ENTITYKEY* pKey, *pNewKey;

	// Destroy our keys.
	this->m_pKeyList->DestroyList();
	
	// Set classname/defintion.
	this->m_sClassName = pEntity->m_sClassName;
	this->m_pEntDef = pEntity->m_pEntDef;

	pEntity->m_pKeyList->First();
	for(pKey = pEntity->m_pKeyList->GetCurItem(); pKey != NULL; pKey = pEntity->m_pKeyList->GetNextItem())
	{
		pNewKey = new ENTITYKEY(*pKey);
		m_pKeyList->AddItem(pNewKey);
	}
}

bool CEntity::In2DRange(vec3_t vMins, vec3_t vMaxs, int nView)
{
	int i;

	BuildRange();

	if(m_pObjectList->IsEmpty() == false)
	{
		CBaseObject* pObj;
		CBaseTransformableObject* pT;

		m_pObjectList->First();
		for(pObj = m_pObjectList->GetCurItem(); pObj != NULL; pObj = m_pObjectList->GetNextItem())
		{
			if((pObj->GetObjectInterfaces()&I_TRANSFORMABLE)==0)
				continue;
			pT = (CBaseTransformableObject*)pObj->GetInterface(I_TRANSFORMABLE);
			if(!pT->In2DRange(vMins, vMaxs, nView))
				return false;
		}

		return true;
	}

	for(i = VEC_X; i <= VEC_Y; i++)
	{
		if(m_vMaxs[VMI(nView, i)] < vMins[VMI(nView, i)] ||
			m_vMins[VMI(nView, i)] > vMaxs[VMI(nView, i)])
				break;
	}

	if(i == VEC_Z)
		return true;
	return false;
}

int CEntity::GetObjectInterfaces()
{ 
	int i = I_TRANSFORMABLE|I_GROUPABLE|I_TEXTUREABLE|I_TARGET|I_VISGROUPREF|I_STORABLE;
	if(m_pObjectList->IsEmpty() == false)
		i |= I_UNLINK|I_FGROUP;
	return i;
}

int CEntity::GetObjectClass()
{
	return (m_pEntDef && m_pEntDef->bOwner) ? OBJECT_CLASS_LENTITY : OBJECT_CLASS_ENTITY;
}

bool CEntity::ReadObject(CFile& hFile, int nVersion)
{
	int i, nLen;
	char sz[256];

	ReadInt(hFile, nLen);
	if(nLen > 255)
		return false;
	hFile.Read(sz, nLen);
	sz[nLen] = NULL;
	m_sClassName = sz;

	i;
	ReadVec3t(hFile, m_vOrigin);

	// Read each key.
	ReadInt(hFile, nLen);
	if(nLen > 0)
	{
		if(nLen > 255)
			return false;

		ENTITYKEY* pKey;
		for(i = 0; i < nLen; i++)
		{
			pKey = ReadKey(hFile, nVersion);
			if(pKey == NULL)
				return false;
			m_pKeyList->AddItem(pKey);
		}
	}

	if(!CBaseTextureableObject::ReadTextureInfo(hFile, nVersion))
		return false;

	// Read the grouped stuff.
	return CObjectGroup::ReadObject(hFile, nVersion);
}

void CEntity::BuildRange()
{
	vec3_t vSize;
	if(m_pEntDef == NULL)
	{
		vSize[0] = 32.0F;
		vSize[1] = 32.0F;
		vSize[2] = 32.0F;
	}
	else
		VectorCopy(m_pEntDef->vSize, vSize);

	InitializeRange(m_vMins, m_vMaxs);
	
	if(m_pObjectList->IsEmpty() == false)
	{
		GDATA gd;
		gd.m_pList = m_pObjectList;
		gd.m_nFlags = GF_RANGE;
		G_Build(&gd);
		InsertRange(gd.m_vMins, m_vMins, m_vMaxs);
		InsertRange(gd.m_vMaxs, m_vMins, m_vMaxs);
		ComputeRangeOrigin(m_vMins, m_vMaxs, m_vOrigin);
	}
	else
	{
		VectorCopy(m_vOrigin, m_vMins);
		VectorCopy(m_vOrigin, m_vMaxs);
		VectorSubtract(m_vMins, vSize, m_vMins);
		VectorAdd(m_vMaxs, vSize, m_vMaxs);
	}
}

bool CEntity::WriteObject(CFile& hFile, int nVersion)
{
	int nLen;

	nLen = m_sClassName.GetLength();
	WriteInt(hFile, nLen);
	hFile.Write((void*)(LPCTSTR)m_sClassName, nLen);

	WriteVec3t(hFile, m_vOrigin);

	// Write each key.
	nLen = m_pKeyList->GetCount();
	hFile.Write(&nLen, 4);
	if(nLen > 0)
	{
		ENTITYKEY* pKey;
		m_pKeyList->First();
		for(pKey = m_pKeyList->GetCurItem(); pKey != NULL; pKey = m_pKeyList->GetNextItem())
		{
			if(!WriteKey(pKey, hFile, nVersion))
				return false;
		}
	}

	if(!CBaseTextureableObject::WriteTextureInfo(hFile, nVersion))
		return false;

	// Write the grouped part, if there is any.
	return CObjectGroup::WriteObject(hFile, nVersion);
}

bool CEntity::WriteKey(ENTITYKEY* pKey, CFile& hFile, int nVersion)
{
	int nLen;

	nLen = pKey->sName.GetLength();
	WriteInt(hFile, nLen);
	hFile.Write((void*)(LPCTSTR)pKey->sName, nLen);
	nLen = pKey->sValue.GetLength();
	WriteInt(hFile, nLen);
	hFile.Write((void*)(LPCTSTR)pKey->sValue, nLen);

	return true;
}

ENTITYKEY* CEntity::ReadKey(CFile& hFile, int nVersion)
{
	int nLen;
	char sz[256];
	ENTITYKEY* pKey = new ENTITYKEY();

	ReadInt(hFile, nLen);
	if(nLen > 255)
	{
		delete pKey;
		return false;
	}
	hFile.Read(sz, nLen);
	sz[nLen] = NULL;
	pKey->sName = sz;

	ReadInt(hFile, nLen);
	if(nLen > 255)
	{
		delete pKey;
		return false;
	}
	hFile.Read(sz, nLen);
	sz[nLen] = NULL;
	pKey->sValue = sz;

	return pKey;
}

int CEntity::InitAfterLoad(CTread3DDoc* pDocument)
{
	int nIAL = CObjectGroup::InitAfterLoad(pDocument);
	if(nIAL != IALF_OK)
		return nIAL;

	// Link the definitions.
	m_pEntDef = GetGameAPI()->GetCurrentGame()->FindEntityDefinition(m_sClassName);
	if(m_pEntDef != NULL)
	{

		ENTITYKEY* pKey;
		m_pKeyList->First();
		for(pKey = m_pKeyList->GetCurItem(); pKey != NULL; pKey = m_pKeyList->GetCurItem())
		{
			pKey->pKeyDef = m_pEntDef->FindKeyDefinition(pKey->sName);
			if(pKey->pKeyDef == NULL)
			{
				Warning("KDNF: class: " + m_pEntDef->sName + " key: " + pKey->sName + ". Key was removed.\n");
				pDocument->AddIALFlags(IALF_NOKEYDEF);
				m_pKeyList->DeleteItem(pKey);
			}
			else
				m_pKeyList->GetNextItem();
		}

		// Add all keys to the entity that the def has.
		m_pEntDef->pKeyList->First();
		ENTITYKEYDEFINITION* pKeyDef;
		for(pKeyDef = m_pEntDef->pKeyList->GetCurItem(); pKeyDef != NULL; pKeyDef = m_pEntDef->pKeyList->GetNextItem())
		{
			pKey = GetKey(pKeyDef->sName);
			if(pKey)
				continue;

			pKey = new ENTITYKEY(pKeyDef);
			m_pKeyList->AddItem(pKey);
		}

		m_nInterfaceRenderFlags = RF_OWNED;

		// hide ourself cause we're an owner?
		/*if(m_pEntDef->bOwner == true)
			m_nObjectRenderFlags = RF_NODRAW;*/
	}
	else
	{
		Warning("CDNF: class " + m_sClassName + "\n", false);
		pDocument->AddIALFlags(IALF_NOENTDEF);
	}

	// What's the classname?
	// If it's the worldspawn we delete ourselves from the map
	// and set ourselves as the worldspawn entity.
	if(GetClassName() == GetGameAPI()->GetCurrentGame()->GetWorldspawnName())
	{
		if(pDocument->GetWorldspawn() != NULL)
		{
			Warning("CEntity: multiple worldspawns!!\n");
			return IALF_DELETE;
		}
		return IALF_REMOVEQ;
	}

	// Setup targetting.
	pDocument->GetTargetAPI()->SetTargetBinding(GetTargetableName(), this);
	//BindToDocument(pDocument);

	return IALF_OK;
}

int CEntity::PostInitAfterLoad(CTread3DDoc* pDocument, int nCode)
{
	if(nCode == IALF_REMOVEQ)
	{
		pDocument->SetWorldspawn(this);
	}

	return IALF_OK;
}

// Create's an entity from a definition.
// It's a handy little constructor, I think.
//CEntity::CEntity(ENTITYDEFINITION* pEntDef) : CObjectGroup()
bool CEntity::InitFromDefinition(ENTITYDEFINITION* pEntDef)
{
	m_pEntDef = pEntDef;
	m_sClassName = pEntDef->sName;
	
	// Allocate a new key list.
	m_pKeyList = new CLinkedList<ENTITYKEY>();

	ENTITYKEYDEFINITION* pKeyDef;
	
	pEntDef->pKeyList->First();
	for(pKeyDef = pEntDef->pKeyList->GetCurItem(); pKeyDef; pKeyDef = pEntDef->pKeyList->GetNextItem())
		m_pKeyList->AddItem(new ENTITYKEY(pKeyDef));
	
	m_nInterfaceRenderFlags = RF_OWNED;

	// Do we hide ourself because we own a structure?
	/*if(pEntDef->bOwner == true)
		m_nObjectRenderFlags = RF_NODRAW;*/

	return true;
}

// Returns the entity key.
ENTITYKEY* CEntity::GetKey(CString sName)
{
	ENTITYKEY* pKey;

	m_pKeyList->First();
	for(pKey = m_pKeyList->GetCurItem(); pKey; pKey = m_pKeyList->GetNextItem())
		if(pKey->sName == sName)
			return pKey;

	return NULL;
}

void* CEntity::GetInterface(int nInterface)
{
	switch(nInterface)
	{
	case I_GROUPABLE:
		return (CBaseGroupableObject*)this;
	case I_TRANSFORMABLE:
		return (CBaseTransformableObject*)this;
	case I_VISGROUPREF:
		return (CVisgroupRefObject*)this;
	case I_GROUP:
		return (CObjectGroup*)this;
	case I_TEXTUREABLE:
		return (CBaseTextureableObject*)this;
	case I_TARGET:
		return (CBaseTargetObject*)this;
	}

	return NULL;
}

bool CEntity::IsOwner()
{
	return (m_pEntDef && m_pEntDef->bOwner);
}

void CEntity::Scale(vec3_t vScale, vec3_t vOrigin)
{
	if(m_pObjectList->IsEmpty() == false)
	{
		GDATA gd;
		gd.m_pList = m_pObjectList;
		VectorCopy(vOrigin, gd.m_vOrigin);
		G_Scale(vScale, &gd);
		BuildRange();
	}
	else
	{
		vec3_t vNegOrg = { -vOrigin[0], -vOrigin[1], -vOrigin[2] };

		Translate(vNegOrg);
		VectorMultiply(m_vOrigin, vScale, m_vOrigin);
		Translate(vOrigin);
	}
}

CString CEntity::GetKeyValue(CString sName)
{
	if(GetKey(sName) == NULL)
		return "";

	return GetKey(sName)->sValue;
}

// From CBaseTransformableObject
void CEntity::Translate(vec3_t vTranslation)
{
	VectorAdd(m_vOrigin, vTranslation, m_vOrigin);
	VectorAdd(m_vMins, vTranslation, m_vMins);
	VectorAdd(m_vMaxs, vTranslation, m_vMaxs);

	if(m_pObjectList->IsEmpty() == false)
	{
		GDATA gd;
		gd.m_pList = m_pObjectList;
		G_Translate(vTranslation, &gd);
	}
}

void CEntity::Rotate(vec3_t vOrigin, vec3_t vRot)
{
	vec3_t vNegOrigin = { -vOrigin[0], -vOrigin[1], -vOrigin[2] };

	Translate(vNegOrigin);
	VectorRotate(m_vOrigin, vRot, m_vOrigin);
	Translate(vOrigin);
	
	if(m_pObjectList->IsEmpty() == false)
	{
		GDATA gd;
		gd.m_pList = m_pObjectList;
		VectorCopy(vOrigin, gd.m_vOrigin);
		G_Rotate(vRot, &gd);
	}
}

void CEntity::InsertIntoRange(vec3_t vMins, vec3_t vMaxs)
{
	BuildRange();

	InsertRange(m_vMins, vMins, vMaxs);
	InsertRange(m_vMaxs, vMins, vMaxs);
}
