// MAPOBJECT.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "Visgroup.h"
#include "TreadSys.h"
#include "Geometry.h"
#include "Brush.h"
#include "Entity.h"
#include "Face.h"
#include "Winding.h"
#include "Camera.h"
#include "PatchMesh.h"
#include "Tread3DDoc.h"
#include "TargetAPI.h"
#include "PluginAPI.h"
#include "Anchor.h"

CBaseObject* NewBaseObject(int nClassType)
{
	CBaseObject* pObj = GetPluginAPI()->CreateBaseObject(nClassType);
	if(pObj != NULL)
		return pObj;

	switch(nClassType)
	{
	case OBJECT_CLASS_GROUP:
		return new CObjectGroup();
	case OBJECT_CLASS_BRUSH:
		return new CQBrush();
	case OBJECT_CLASS_ENTITY:
	case OBJECT_CLASS_LENTITY:
		return new CEntity();
	case OBJECT_CLASS_FACE:
		return new CFace();
	case OBJECT_CLASS_WINDING:
		return new CWinding();
	case OBJECT_CLASS_PATCHMESH:
		return new CPatchMesh();
	case OBJECT_CLASS_CAMERA:
		return new CCamera();
	case OBJECT_CLASS_ANCHOR:
		return new CAnchor();
	}

	return NULL;
}

CBaseObject* CBaseObject::Clone()
{
	Warning("Cloning base object!\n");
	return new CBaseObject(*this);
}

// Reads the texture info.
bool CBaseTextureableObject::ReadTextureInfo(CFile& hFile, int nVersion)
{
	int i;
	int nLen;
	char szName[256];

	DetachTexture();

	// Load the texture name.
	if(!hFile.Read(&nLen, 4))
		return false;
	if(nLen > 0)
	{
		if(nLen > 255)
			return false;
		szName[nLen] = 0;
		if(!hFile.Read(szName, nLen))
			return false;

		m_sTexture = szName;
	}
	else
		m_sTexture = "";

	if(!hFile.Read(&m_vRotation, 4))
		return false;
	for(i = 0; i < 2; i++)
		if(!hFile.Read(&m_vTexShifts[i], 4))
			return false;
	for(i = 0; i < 2; i++)
		if(!hFile.Read(&m_vTexScales[i], 4))
			return false;

	if(nVersion > _2X_HEADER_VERSION_2)
	{
		hFile.Read(&m_nSurface, 4);
		hFile.Read(&m_nContents, 4);
		hFile.Read(&m_nValue, 4);
	}

	return true;
}

// Writes the texture info.
bool CBaseTextureableObject::WriteTextureInfo(CFile& hFile, int nVersion)
{
	int i;
	int nLen;

	// Name.
	nLen = m_sTexture.GetLength();
	hFile.Write(&nLen, 4);
	if(nLen > 0)
		hFile.Write((LPCTSTR)m_sTexture, nLen);

	hFile.Write(&m_vRotation, 4);
	for(i = 0; i < 2; i++)
		hFile.Write(&m_vTexShifts[i], 4);
	for(i = 0; i < 2; i++)
		hFile.Write(&m_vTexScales[i], 4);

	hFile.Write(&m_nSurface, 4);
	hFile.Write(&m_nContents, 4);
	hFile.Write(&m_nValue, 4);

	return true;
}

// Builds the texture reference.
bool CBaseTextureableObject::AttachTexture()
{
	if(m_sTexture == "")
		return false;
	
	if(m_pTexture)
	{
		if(m_pTexture->m_sName == m_sTexture)
			return false;
	}

	// ... LOOK UP TEXTURE BASED ON NAME.
	m_pTexture = GetTexAPI()->GetTextureFromName(m_sTexture);
	if(m_pTexture != NULL)
		m_pTexture->LoadInfo();

	return m_pTexture != NULL;
}

// Detaches the texture.
bool CBaseTextureableObject::DetachTexture()
{
	m_pTexture = NULL;
	return true;
}

CObjectGroup::CObjectGroup() : CBaseGroupableObject(), CVisgroupRefObject(), CBaseTransformableObject(), CBaseTextureableObject()
{
	m_pObjectList = new CLinkedList<CBaseObject>();
}

CObjectGroup::CObjectGroup(const CObjectGroup& ObjectToCopy) : CBaseGroupableObject(ObjectToCopy), CVisgroupRefObject(ObjectToCopy), CBaseTransformableObject(ObjectToCopy), CBaseTextureableObject(ObjectToCopy)
{
	m_pObjectList = new CLinkedList<CBaseObject>();
	G_CopyList(ObjectToCopy.m_pObjectList, m_pObjectList);
	CBaseObject* pObj;
	CBaseGroupableObject* pGObj;
	m_pObjectList->First();
	for(pObj = m_pObjectList->GetCurItem(); pObj != NULL; pObj = m_pObjectList->GetNextItem())
	{
		pGObj = (CBaseGroupableObject*)pObj->GetInterface(I_GROUPABLE);
		pGObj->m_pGroup = this;
	}
	m_nObjectRenderFlags &= ~RF_OWNED;
	m_nInterfaceRenderFlags &= ~RF_OWNED;
}

CObjectGroup::~CObjectGroup()
{
	if(m_pObjectList)
		delete m_pObjectList;

	m_pObjectList = NULL;
}

bool CObjectGroup::In2DRange(vec3_t vMins, vec3_t vMaxs, int nView)
{
	CBaseObject* pObj;
	CBaseTransformableObject* pT;

	m_pObjectList->First();
	for(pObj = m_pObjectList->GetCurItem(); pObj != NULL; pObj = m_pObjectList->GetNextItem())
	{
		if(pObj->GetObjectInterfaces()&I_TRANSFORMABLE)
		{
			pT = (CBaseTransformableObject*)pObj->GetInterface(I_TRANSFORMABLE);
			if(pT->In2DRange(vMins, vMaxs, nView))
				return true;
		}
	}

	return false;
}

bool CObjectGroup::ObjectsIntersect(CBaseObject* pObject)
{
	CBaseTransformableObject* pT = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
	if(pT == NULL)
		return false;

	BuildRange();
	pT->BuildRange();

	if(!RangesIntersect(m_vMins, m_vMaxs, pT->m_vMins, pT->m_vMaxs))
		return false;

	// See if any object in the group hits the source object.
	CBaseObject* pObj;
	CBaseCarveableObject* pC;

	m_pObjectList->First();
	for(pObj = m_pObjectList->GetCurItem(); pObj != NULL; pObj = m_pObjectList->GetNextItem())
	{
		pC = (CBaseCarveableObject*)pObj->GetInterface(I_CARVEABLE);
		if(pC == NULL)
			continue;

		if(pC->ObjectsIntersect(pObject))
			return true;
	}

	return false;
}

CObjectGroup* CObjectGroup::CSGSubtract(CBaseObject* pCarveObject)
{
	if(pCarveObject->GetObjectClass() == OBJECT_CLASS_GROUP)
		return CObjectGroup::CarveWithGroup(this, (CObjectGroup*)pCarveObject);
	
	CObjectGroup* pGroup=NULL;
	CObjectGroup* pRejectGroup = new CObjectGroup();
	CObjectGroup* pMainGroup = new CObjectGroup();
	CBaseObject* pObject;
	
	CBaseTransformableObject* pCarveT = (CBaseTransformableObject*)pCarveObject->GetInterface(I_TRANSFORMABLE);
	if(pCarveT == NULL)
		return NULL;
	pCarveT->BuildRange();

	// Loop through each object and carve it.
	m_pObjectList->First();
	for(;;)
	{
		pObject = m_pObjectList->GetCurItem();
		if(pObject == NULL)
			break;
		if(pObject->GetObjectInterfaces()&I_CARVEABLE)
		{
			// Test ranges.
			CBaseTransformableObject* pT = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
			if(pT == NULL)
			{
				Warning("CObjectGroup::CSGSubtract(): I_CARVEABLE w/o I_TRANSFORMABLE\n");
				continue;
			}

			pT->BuildRange();
			CBaseCarveableObject* pC = (CBaseCarveableObject*)pObject->GetInterface(I_CARVEABLE);

			if(pC->ObjectsIntersect(pCarveObject))
			{
				// Carve.
				pGroup = pC->CSGSubtract(pCarveObject);
				if(pGroup != NULL)
					pMainGroup->AddObject(pGroup);
				else
					Warning("CObjectGroup::CSGSubtract(): subtract returned NULL!!\n");
				m_pObjectList->GetNextItem();
			}
			else
			{
				pObject = pObject->Clone();
				m_pObjectList->GetNextItem();
				pRejectGroup->AddObject((CBaseGroupableObject*)pObject);
			}
		}
		else
			m_pObjectList->GetNextItem();
	}

	if(pRejectGroup->GetObjectList()->IsEmpty())
	{
		delete pRejectGroup;
		pRejectGroup = NULL;
	}
	
	if(pRejectGroup != NULL)
	{
		if(pRejectGroup->GetObjectList()->GetCount() == 1)
		{
			// Remove the object and add to the main list.
			CBaseObject* pObj = pRejectGroup->GetObjectList()->GetItem(LL_HEAD);
			pRejectGroup->RemoveObject((CBaseGroupableObject*)pObj);
			pMainGroup->AddObject((CBaseGroupableObject*)pObj);
			delete pRejectGroup;
			pRejectGroup = NULL;
		}
		else
			pMainGroup->AddObject(pRejectGroup);
	}

	// Grouping only a single object is useless.
	if(pMainGroup->GetObjectList()->GetCount() == 1)
	{
		CBaseObject* pObj = pMainGroup->GetObjectList()->GetItem(LL_HEAD);
		if(pObj->GetObjectClass() != OBJECT_CLASS_GROUP)
			Warning("CObjectGroup::CSGSubtract(): group containes single object!!!\n");
		else
		{
			pMainGroup->RemoveObject((CBaseGroupableObject*)pObj);
			delete pMainGroup;
			pMainGroup = (CObjectGroup*)pObj;
		}
	}

	if(pMainGroup->GetObjectList()->IsEmpty())
	{
		delete pMainGroup;
		pMainGroup = NULL;
	}
	
	return pMainGroup;
}

CObjectGroup* CObjectGroup::CarveWithGroup(CBaseCarveableObject* pCarveTarget, CObjectGroup* pCarveGroup)
{
	CBaseObject* pObject;
	CObjectGroup* pGroup=NULL;
	CObjectGroup* pMainGroup = new CObjectGroup();
	CLinkedList<CBaseObject>* pList = pCarveGroup->GetObjectList();

	pList->First();
	for(pObject = pList->GetCurItem(); pObject != NULL ; pObject = pList->GetNextItem())
	{
		if((pObject->GetObjectInterfaces()&I_CARVE)==0)
			continue;

		if(pGroup == NULL)
		{
			// Does the target carve support the source object class?
			if(pCarveTarget->GetCarveClasses()&pObject->GetObjectClass())
				pGroup = pCarveTarget->CSGSubtract(pObject);
		}
		else
		if(pGroup->GetCarveClasses()&pObject->GetObjectClass())
			pGroup = pGroup->CSGSubtract(pObject);

		if(pGroup != NULL)
			pMainGroup->AddObject(pGroup);
	}

	if(pMainGroup->GetObjectList()->IsEmpty())
	{
		delete pMainGroup;
		pMainGroup = NULL;
	}

	return pMainGroup;
}

void CObjectGroup::Reflect(int nAxis, vec3_t vFlipOrigin)
{
	CBaseObject* pObject;

	m_pObjectList->First();
	for(pObject = m_pObjectList->GetCurItem(); pObject != NULL; pObject = m_pObjectList->GetNextItem())
	{
		if(pObject->GetObjectInterfaces()&I_TRANSFORMABLE)
		{
			CBaseTransformableObject* pT = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
			pT->Reflect(nAxis, vFlipOrigin);
		}
	}
}

int CObjectGroup::InitAfterLoad(CTread3DDoc* pDocument)
{
	BindToDocument(pDocument);

	// Go through each object and do post load initialization.
	int alf;
	CBaseObject* pObject;
	m_pObjectList->First();
	for(;;)
	{
		pObject = m_pObjectList->GetCurItem();
		if(pObject == NULL)
			break;

		alf = pObject->InitAfterLoad(pDocument);
		switch(alf)
		{
		case IALF_OK:

			m_pObjectList->GetNextItem();
			break;
	
		case IALF_RESTART:

			m_pObjectList->First();
			continue;

		case IALF_DELETE:
		case IALF_DELETEQ:

			if(alf == IALF_DELETE)
			{
				Warning("IALF: Object deleted from map due to IALF request.\n");
				Debug("IALF: Object deleted from map due to IALF request.\n");
			}
			m_pObjectList->DeleteItem(LL_CURRENT);
			break;

		case IALF_REMOVE:
		case IALF_REMOVEQ:

			if(alf == IALF_REMOVE)
			{
				Warning("IALF: Object removed from map due to IALF request.\n");
				Debug("IALF: Object removed from map due to IALF request.\n");
			}

			m_pObjectList->RemoveItem(LL_CURRENT);
			break;

		case IALF_FATAL:

			return IALF_FATAL;
		}
		
		if(alf != IALF_DELETE && alf != IALF_DELETEQ)
		{
			if(pObject->PostInitAfterLoad(pDocument, alf) == IALF_FATAL)
				return IALF_FATAL;
		}
	}

	return IALF_OK;
}

// Storable Object.
bool CObjectGroup::ReadObject(CFile& hFile, int nVersion)
{
	int nNumObjects;
	CLinkedList<CBaseObject>* pList = new CLinkedList<CBaseObject>();

	hFile.Read(&nNumObjects, 4);
	if(nNumObjects > 4096)
	{
		Debug("CObjectGroup::ReadObject(): more than 4096 objects in group!!\n");
		return false;
	}

	if(!G_ReadObjectList(nNumObjects, nVersion, hFile, pList, NULL))
	{
		delete pList;
		return NULL;
	}

	// Add each object to the group.
	CBaseObject* pObject;
	pList->First();
	for(;;)
	{
		pObject = pList->RemoveItem(LL_HEAD);
		if(pObject == NULL)
			break;
		if((pObject->GetObjectInterfaces()&I_GROUPABLE)==0)
		{
			Warning("CObjectGroup::ReadObject(): object is not groupable!!!\n");
			delete pObject;
			continue;
		}
		AddObject((CBaseGroupableObject*)pObject->GetInterface(I_GROUPABLE));
	}

	delete pList;

	if(nVersion > _2X_HEADER_VERSION_7)
		if(!CVisgroupRefObject::Read(hFile))
			return false;

	return true;
}

bool CObjectGroup::WriteObject(CFile& hFile, int nVersion)
{
	// Write the object lists.
	int nNumObjects = G_GetInterfaceCount(m_pObjectList, I_STORABLE);
	hFile.Write(&nNumObjects, 4);
	if(!G_WriteObjectList(nVersion, hFile, m_pObjectList))
		return false;

	return CVisgroupRefObject::Write(hFile);
}

void CObjectGroup::OnTextureChange()
{
	G_TextureList(m_pObjectList, m_sTexture);
}

void CObjectGroup::SetOrigin(vec3_t vOrigin)
{
	GDATA GData;

	GData.m_nFlags = GF_NONE;
	GData.m_pList = m_pObjectList;
	VectorCopy(m_vOrigin, GData.m_vOrigin);
	G_SetOrigin(vOrigin, &GData);
}

void CObjectGroup::Scale(vec3_t vScale)
{
	GDATA GData;

	GData.m_nFlags = GF_NONE;
	GData.m_pList = m_pObjectList;
	VectorCopy(m_vOrigin, GData.m_vOrigin);
	G_Scale(vScale, &GData);
}

void CObjectGroup::Scale(vec3_t vScale, vec3_t vOrigin)
{
	GDATA GData;

	GData.m_nFlags = GF_NONE;
	GData.m_pList = m_pObjectList;
	VectorCopy(vOrigin, GData.m_vOrigin);
	G_Scale(vScale, &GData);
}

void CObjectGroup::Translate(vec3_t vTranslation)
{
	GDATA GData;

	GData.m_nFlags = GF_NONE;
	GData.m_pList = m_pObjectList;
	G_Translate(vTranslation, &GData);
}

void CObjectGroup::Rotate(vec3_t vRot)
{
	GDATA GData;

	GData.m_nFlags = GF_NONE;
	GData.m_pList = m_pObjectList;
	VectorCopy(m_vOrigin, GData.m_vOrigin);
	G_Rotate(vRot, &GData);
}

void CObjectGroup::Rotate(vec3_t vOrigin, vec3_t vRot)
{
	GDATA GData;

	GData.m_nFlags = GF_NONE;
	GData.m_pList = m_pObjectList;
	VectorCopy(vOrigin, GData.m_vOrigin);
	G_Rotate(vRot, &GData);
}

void CObjectGroup::InsertIntoRange(vec3_t vMins, vec3_t vMaxs)
{
	BuildRange();
	InsertRange(m_vMins, vMins, vMaxs);
	InsertRange(m_vMaxs, vMins, vMaxs);
}

void CObjectGroup::ComputeOrigin()
{
	ComputeRangeOrigin(m_vMins, m_vMaxs, m_vOrigin);
}

void CObjectGroup::BuildRange()
{
	GDATA GData;

	GData.m_nFlags = GF_RANGE;
	GData.m_pList = m_pObjectList;
	G_Build(&GData);

	VectorCopy(GData.m_vMins, m_vMins);
	VectorCopy(GData.m_vMaxs, m_vMaxs);
}

// Get'ts an interface
void* CObjectGroup::GetInterface(int nInterface)
{
	switch(nInterface)
	{
	case I_GROUP:
		return this;
	case I_GROUPABLE:
		return (CBaseGroupableObject*)this;
	case I_TRANSFORMABLE:
		return (CBaseTransformableObject*)this;
	case I_TEXTUREABLE:
		return (CBaseTextureableObject*)this;
	case I_VISGROUPREF:
		return (CVisgroupRefObject*)this;
	case I_CARVEABLE:
		return (CBaseCarveableObject*)this;
	}

	return NULL;
}

CObjectGroup* CBaseGroupableObject::GetHeadGroup()
{
	CObjectGroup* pGroup=m_pGroup;
	if(!pGroup)
		return NULL;

	for(;;)
	{
		if(pGroup->GetParentGroup()==NULL)
			return pGroup;
		pGroup = pGroup->GetParentGroup();
	}

	return NULL;
}

void* CBaseObject::GetInterface(int nInterface)
{
	Error("CBaseObject::GetInterface(): is not overriden!!\nThis may cause a crash.");
	return this;
}

void CBaseTextureableObject::SetTextureName(LPCTSTR szName, bool bEvent) 
{ 
	m_sTexture = szName; 
	m_sTexture.MakeLower();
	if(bEvent)
	{
		OnTextureChange();
		OnAttributeChange();
	}
}

// Add's an object to our list.
bool CObjectGroup::AddObject(CBaseGroupableObject* pObject)
{
	if(!m_pObjectList->AddItem(pObject))
		return false;

	pObject->m_pGroup = this;

	return true;
}

// Removes an object from our list.
bool CObjectGroup::RemoveObject(CBaseGroupableObject* pObject)
{
	if(!m_pObjectList->RemoveItem(pObject))
		return false;

	pObject->m_pGroup = NULL;

	return true;
}


bool CVisgroupRef::Read(CFile& hFile)
{
	int nLen;
	char szName[256];

	hFile.Read(&nLen, 4);
	if(nLen > 0 && nLen < 255)
	{
		hFile.Read(szName, nLen);
		szName[nLen] = 0;
		m_sName = szName;
	}
	else
		m_sName = "";

	hFile.Read(&nLen, 4);
	if(nLen > 0 && nLen < 255)
	{
		hFile.Read(szName, nLen);
		szName[nLen] = 0;
		m_sCategory = szName;
	}
	else
		m_sCategory = "";

	return true;
}

bool CVisgroupRef::Write(CFile& hFile)
{
	int nLen = GetName().GetLength();
	hFile.Write(&nLen, 4);
	if(nLen > 0)
		hFile.Write((LPCTSTR)GetName(), nLen);
	nLen = GetCategory().GetLength();
	hFile.Write(&nLen, 4);
	if(nLen > 0)
		hFile.Write((LPCTSTR)GetCategory(), nLen);

	return true;
}

// Visgroup Reference.
bool CVisgroupRef::IsVisible()
{
	if(m_pVisgroup == NULL)
		return true;
	
	return m_pVisgroup->GetVisible();
}

// Get's the name and cat.
CString CVisgroupRef::GetName()
{
	if(m_pVisgroup == NULL)
		return "";

	return m_pVisgroup->GetName();
}

CString CVisgroupRef::GetCategory()
{
	if(m_pVisgroup == NULL)
		return "";

	return m_pVisgroup->GetCategory();
}

void CVisgroupRef::SetVisgroup(CString sName, CString sCategory)
{
	if(m_pDocument == NULL)
	{
		Error("CVisgroupRef::SetVisgroup(): not bound to document");
		return;
	}

	SetVisgroup(m_pDocument->GetVisgroupAPI()->FindVisgroup(sName, sCategory));
}

// Bind it.
void CVisgroupRef::BindToDocument(CTread3DDoc* pDocument)
{
	if(m_pDocument == pDocument)
		return;
	if(m_pDocument)
	{
		// Unbind.
		m_pDocument->GetVisgroupAPI()->DeleteVisgroupRef(this);
	}

	m_pDocument = pDocument;
	pDocument->GetVisgroupAPI()->AddVisgroupRef(this);

	// Set the visgroup.
	if(m_sName != "" && m_sCategory != "")
		SetVisgroup(m_sName, m_sCategory);
}

CVisgroupRef::CVisgroupRef()
{
	m_pVisgroup = NULL;
	m_pDocument = NULL;
}

CVisgroupRef::CVisgroupRef(const CVisgroupRef& VisgroupRefToCopy)
{
	m_pVisgroup = VisgroupRefToCopy.m_pVisgroup;
	m_pDocument = VisgroupRefToCopy.m_pDocument;
}

CVisgroupRef::~CVisgroupRef()
{
	if(m_pDocument)
		m_pDocument->GetVisgroupAPI()->DeleteVisgroupRef(this);
}