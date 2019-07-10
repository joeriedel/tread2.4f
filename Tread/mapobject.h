// MAPOBJECT.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "LinkedList.h"
#include "MathLib.h"
#include "Texture.h"
#include "TexAPI.h"
#include "TreadSys.h"

// $sb "mapobject.h block1"
#define OBJECT_CLASS_UNKNOWN			-1
#define OBJECT_CLASS_ENTITY				0x00000001
#define OBJECT_CLASS_BRUSH				0x00000002
#define OBJECT_CLASS_FACE				0x00000004
#define OBJECT_CLASS_WINDING			0x00000008
#define OBJECT_CLASS_PATCHMESH			0x00000010
#define OBJECT_CLASS_GROUP				0x00000020
#define OBJECT_CLASS_BASE				0x00000040
#define OBJECT_CLASS_CAMERA				0x00000080
#define OBJECT_CLASS_HOTSPOT			0x00000100
#define OBJECT_CLASS_CLIPTOOL			0x00000200
#define OBJECT_CLASS_LENTITY			0x00000400
#define OBJECT_CLASS_ANCHOR				0x00000800

// We reserve object id's up to 0x00800000
// Please do not use any numbers up to and
// including this define.
#define OBJECT_CLASS_RESERVED_LAST		0x00800000
#define OBJECT_CLASS_RESERVED_MASK		0x00FFFFFF
#define OBJECT_CLASS_USER_MASK			0xFF000000

// These bits are free for use.
#define OBJECT_CLASS_USER1				0x01000000
#define OBJECT_CLASS_USER2				0x02000000
#define OBJECT_CLASS_USER3				0x04000000
#define OBJECT_CLASS_USER4				0x08000000
#define OBJECT_CLASS_USER5				0x10000000
#define OBJECT_CLASS_USER6				0x20000000
#define OBJECT_CLASS_USER7				0x40000000
#define OBJECT_CLASS_USER8				0x80000000

#define OBJECT_CLASS_ALL				0xFFFFFFFF

#define I_NONE			0x00000000
#define I_BASE			0x00000001
#define I_STORABLE		0x00000002
#define I_TRANSFORMABLE	0x00000004
#define I_TEXTUREABLE	0x00000008
#define I_GROUPABLE		0x00000010
#define I_CHECKABLE		0x00000020
#define I_VISGROUPREF	0x00000040
#define I_GROUP			0x00000080
#define I_HSPOTGEN		0x00000100
#define I_UI			0x00000200	// User Interface Object
#define I_CARVE			0x00000400	// Can carve with it.
#define I_HOLLOW		0x00000800	// Can be hollowed.
#define I_CARVEABLE		0x00001000	// Can be carved.
#define I_TEXADJUST		0x00002000	// Texture can be adjusted via face dialog.
#define I_TARGET		0x00004000	// Target.
#define I_LINK			0x00008000	// Linkable.
#define I_UNLINK		0x00010000	// Unlinkable.
#define I_FGROUP		0x00020000	

#define I_RESERVED_LAST 0x00800000
#define I_RESERVED_MASK	0x00FFFFFF
#define I_USER_MASK		0xFF000000
#define I_USER1			0x01000000
#define I_USER2			0x02000000
#define I_USER3			0x04000000
#define I_USER4			0x08000000
#define I_USER5			0x10000000
#define I_USER6			0x20000000
#define I_USER7			0x40000000
#define I_USER8			0x80000000

#define I_ALL			0xFFFFFFFF

// These macro's ease the casting of object interfaces.

#define ICT_GEN(a, i)	((a->GetObjectInterfaces(i)&i)==i)
#define  IC_GEN(a, i)	(a->GetInterface(i))

#define ICT_BASE(a)					ICT_GEN(a, I_BASE)
#define  IC_BASE(a)					 IC_GEN(a, I_BASE)
#define ICT_STORABLE(a)				ICT_GEN(a, I_STORABLE)
#define  IC_STORABLE(a)				(CBaseStorableObject*)IC_GEN(a, I_STORABLE)
#define ICT_TRANSFORMABLE(a)		ICT_GEN(a, I_TRANSFORMABLE)
#define  IC_TRANSFORMABLE(a)		(CBaseTransformableObject*)IC_GEN(a, I_TRANSFORMABLE)
#define ICT_TEXTUREABLE(a)			ICT_GEN(a, I_TEXTUREABLE)
#define  IC_TEXTUREABLE(a)			(CBaseTextureableObject*)IC_GEN(a, I_TEXTUREABLE)
#define ICT_GROUPABLE(a)			ICT_GEN(a, I_GROUPABLE)
#define  IC_GROUPABLE(a)			(CBaseGroupableObject)IC_GEN(a, I_GROUPABLE)
#define ICT_CHECKABLE(a)			ICT_GEN(a, I_CHECKABLE)
#define  IC_CHECKABLE(a)			(CBaseCheckableObject*)IC_GEN(a, I_CHECKABLE)
#define ICT_VISGROUPREF(a)			ICT_GEN(a, I_VISGROUPREF)
#define  IC_VISGROUPREF(a)			(CVisgroupRef*)IC_GEN(a, I_VISGROUPREF)
#define ICT_GROUP(a)				ICT_GEN(a, I_GROUP)
#define  IC_GROUP(a)				(CObjectGroup*)IC_GEN(a, I_GROUP)
#define ICT_HSPOTGEN(a)				ICT_GEN(a, I_HSPOTGEN)
#define  IC_HSPOTGEN(a)				(CBaseHotSpotGenerator*)IC_GEN(a, I_HSPOTGEN)
#define ICT_UI(a)					ICT_GEN(a, I_UI)
#define ICT_CARVE(a)				ICT_GEN(a, I_CARVE)
#define ICT_HOLLOW(a)				ICT_GEN(a, I_HOLLOW)
#define ICT_CARVEABLE(a)			ICT_GEN(a, I_CARVEABLE)
#define  IC_CARVEABLE(a)			(CBaseCarveableObject*)IC_GEN(a, I_CARVEABLE)
#define ICT_TEXADJUST(a)			ICT_GEN(a, I_TEXADJUST)
#define ICT_TARGET(a)				ICT_GEN(a, I_TARGET)
#define  IC_TARGET(a)				(CBaseTargetObject*)IC_GEN(a, I_TARGET)
#define ICT_LINK(a)					ICT_GEN(a, I_LINK)
#define ICT_UNLINK(a)				ICT_GEN(a, I_UNLINK)
#define ICT_FGROUP(a)				ICT_GEN(a, I_FGROUP)

class CTread3DDoc;

// A storable object.
class _T_EXPORT CBaseStorableObject
{
public:

	CBaseStorableObject() {}
	virtual ~CBaseStorableObject() {}

	virtual bool ReadObject(CFile& hFile, int nVersion) = 0;
	virtual bool WriteObject(CFile& hFile, int nVersion) = 0;

	// If a higher object wants to be read/written it should
	// extend this to return the I_STORABLE interface.
	virtual int GetObjectInterfaces() = 0;
	virtual int GetObjectClass() = 0;

	// Writes object id.
	bool WriteObjectClass(CFile& hFile)
	{
		int nId;
		
		nId = GetObjectClass();
		hFile.Write(&nId, 4);	

		return true;
	}

	// Read's an object class.
	static int ReadObjectClass(CFile& hFile)
	{
		int nId;
		hFile.Read(&nId, 4);
		return nId;
	}

};

// A Base Object.
class _T_EXPORT CBaseObject : public CLLObject, public CBaseStorableObject
{
public:

	int m_nObjectRenderFlags;
	int m_nClassRenderFlags;
	int m_nInterfaceRenderFlags;
	int m_nViewTypeFilters;

	CBaseObject() : CLLObject(), CBaseStorableObject()
	{ 
		m_nObjectRenderFlags = 0; 
		m_nClassRenderFlags = 0;
		m_nInterfaceRenderFlags = 0;
		m_nViewTypeFilters = 0;
	}

	CBaseObject(const CBaseObject& ObjectToCopy) : CLLObject(ObjectToCopy), CBaseStorableObject()
	{
		m_nObjectRenderFlags = ObjectToCopy.m_nObjectRenderFlags;
		m_nClassRenderFlags = ObjectToCopy.m_nClassRenderFlags;
		m_nInterfaceRenderFlags = ObjectToCopy.m_nInterfaceRenderFlags;
		m_nViewTypeFilters = ObjectToCopy.m_nViewTypeFilters;
	}

	virtual ~CBaseObject()
	{ 
	}
	
	// Called before object is deleted.
	virtual void PreDelete(CTread3DDoc* pDocument) {}
	// Called when an action is undone on the object.
	virtual void Undo(int nFlags, CTread3DDoc* pDocument) {}
	// Called when an action is redone on the object.
	virtual void Redo(int nFlags, CTread3DDoc* pDocument) {}

	// Get's the object class.
	virtual int GetObjectClass() { return OBJECT_CLASS_BASE; }

	virtual CBaseObject* Clone();

#define IALF_OK			0x00000000
#define IALF_REMOVE		0x00000001
#define IALF_REMOVEQ	0x00000002	// Quite remove.
#define IALF_FATAL		0x00000004
#define IALF_NOENTDEF	0x00000008
#define IALF_NOKEYDEF	0x00000010
#define IALF_NOTEXTURES	0x00000020
#define IALF_DELETE		0x00000040
#define IALF_DELETEQ	0x00000080
#define IALF_RESTART	0x00000100
#define IALF_END		0x00000200
#define IALF_ALL		0xFFFFFFFF

	virtual int InitAfterLoad(CTread3DDoc* pDocument) { return IALF_OK; }
	virtual int PostInitAfterLoad(CTread3DDoc* pDocument, int nCode) { return IALF_OK; }

	virtual void InitFromSelection(CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin) {}

	// If an higher object wants to be read/written it should
	// extend this to return the I_STORABLE interface.
	// This defines what interface classes this object can be
	// cast to.
	virtual int GetObjectInterfaces(){ return I_BASE; }
	virtual void* GetInterface(int nInterface);

	// Storable Object.
	virtual bool ReadObject(CFile& hFile, int nVersion) { return false; }
	virtual bool WriteObject(CFile& hFile, int nVersion) { return false; }

};

// A groupable object is an object that can be stored in 
// a CObjectGroup class.

class _T_EXPORT CObjectGroup;
class _T_EXPORT CBaseGroupableObject : public CBaseObject
{
protected:

	CObjectGroup* m_pGroup;

public:
	
	CBaseGroupableObject() : CBaseObject() { m_pGroup = NULL; }
	CBaseGroupableObject(const CBaseGroupableObject&  ObjectToCopy) : CBaseObject(ObjectToCopy)
	{ m_pGroup = ObjectToCopy.m_pGroup; }
	virtual ~CBaseGroupableObject() {}

	virtual CBaseObject* Clone() { return new CBaseGroupableObject(*this); }

	CObjectGroup* GetParentGroup() { return m_pGroup; }
	CObjectGroup* GetHeadGroup();
	virtual int GetObjectInterfaces() { return I_BASE | I_GROUPABLE; }
	friend class CObjectGroup;
};

// A carvable object.
class _T_EXPORT CObjectGroup;
class _T_EXPORT CBaseCarveableObject
{
public:

	CBaseCarveableObject() {}
	virtual ~CBaseCarveableObject() {}

	virtual CObjectGroup* CSGSubtract(CBaseObject* pObject) { return NULL; }
	virtual bool ObjectsIntersect(CBaseObject* pObject) { return false; }
	virtual int GetCarveClasses() const { return 0; }
};

// Target object.
// I_TARGETABLE
class _T_EXPORT CBaseTargetObject
{
public:

	CBaseTargetObject() {}
	virtual ~CBaseTargetObject() {}

	virtual CString GetTargetableName() = 0;
	virtual void SetTargetableName(CString sName, CTread3DDoc* pDocument) = 0;
	virtual CString GetTargetName() = 0;
	virtual void SetTargetName(CString sName, CTread3DDoc* pDocument) = 0;

};


// An object that can be transformed.
class _T_EXPORT CBaseTransformableObject
{
public:

	// Data
	vec3_t m_vOrigin;
	vec3_t m_vMins, m_vMaxs;

	// Methods.
	CBaseTransformableObject()
	{
		m_vOrigin[0] = 0.0F;
		m_vOrigin[1] = 0.0F;
		m_vOrigin[2] = 0.0F;
	}

	CBaseTransformableObject(const CBaseTransformableObject& ObjectToCopy)
	{ VectorCopy(ObjectToCopy.m_vOrigin, m_vOrigin); }

	virtual ~CBaseTransformableObject() {}

	virtual void SetOrigin(vec3_t vOrigin) {}
	virtual void Scale(vec3_t vScale) {}	// Scales the objects size.
	virtual void Scale(vec3_t vScale, vec3_t vOrigin) {}
	virtual void Translate(vec3_t vTranslation) {}
	virtual void Rotate(vec3_t vRot) {}
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot) {}
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs) {}
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin) {}
	virtual void ComputeOrigin() {}
	virtual void BuildRange() {}
	virtual bool InRange(vec3_t vMins, vec3_t vMaxs) { return false; }
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView) { return false; }
};


// Defines a textureable object.
class _T_EXPORT CBaseTextureableObject
{
protected:

	CString	m_sTexture;
	CTexture* m_pTexture;
	vec_t	  m_vRotation;
	vec_t	  m_vTexShifts[2];
	vec_t	  m_vTexScales[2];
	
	// Writes the texture info.
	bool WriteTextureInfo(CFile& hFile, int nVersion);

	// Reads the texture info.
	bool ReadTextureInfo(CFile& hFile, int nVersion);

	// Builds the texture reference.
	bool AttachTexture();

	// Detaches the texture.
	bool DetachTexture();

public:

	int	m_nSurface;
	int m_nContents;
	int m_nValue;

	CBaseTextureableObject()
	{
		m_sTexture = "";
		m_pTexture = NULL;
		m_vRotation = 0.0F;
		m_vTexShifts[0] = 0.0F; m_vTexShifts[1] = 0.0F;
		m_vTexScales[0] = 1.0F; m_vTexScales[1] = 1.0F;
		m_nSurface = 0;
		m_nContents = 0;
		m_nValue = 0;
	}

	CBaseTextureableObject(const CBaseTextureableObject& ObjectToCopy)
	{
		m_sTexture = ObjectToCopy.m_sTexture;
		m_pTexture = ObjectToCopy.m_pTexture;
		m_vRotation = ObjectToCopy.m_vRotation;
		m_vTexShifts[0] = ObjectToCopy.m_vTexShifts[0];
		m_vTexShifts[1] = ObjectToCopy.m_vTexShifts[1];
		m_vTexScales[0] = ObjectToCopy.m_vTexScales[0];
		m_vTexScales[1] = ObjectToCopy.m_vTexScales[1];
		m_nSurface = ObjectToCopy.m_nSurface;
		m_nContents = ObjectToCopy.m_nContents;
		m_nValue = ObjectToCopy.m_nValue;
	}
	
	virtual ~CBaseTextureableObject() {}

	void SetRotation(vec_t vRotation, bool bRebuild=true)
	{ 
		m_vRotation = vRotation; 
		if(bRebuild)
			RebuildTexture();
	}

	vec_t GetRotation() { return m_vRotation; }

	void SetTexShifts(vec_t vTexShifts[2], bool bRebuild=true)
	{
		m_vTexShifts[0] = vTexShifts[0];
		m_vTexShifts[1] = vTexShifts[1];
		if(bRebuild)
			RebuildTexture();
	}

	void GetTexShifts(vec_t vTexShifts[2])
	{
		vTexShifts[0] = m_vTexShifts[0];
		vTexShifts[1] = m_vTexShifts[1];
	}

	void SetTexScales(vec_t vTexScales[2], bool bRebuild=true)
	{
		m_vTexScales[0] = vTexScales[0];
		m_vTexScales[1] = vTexScales[1];
		if(bRebuild)
			RebuildTexture();
	}
	
	void GetTexScales(vec_t vTexScales[2])
	{
		vTexScales[0] = m_vTexScales[0];
		vTexScales[1] = m_vTexScales[1];
	}

	CString GetTextureName() { return m_sTexture; }
	void SetTextureName(const char* szName, bool bEvent=true);
	virtual CTexture* GetTexturePtr() { AttachTexture(); return m_pTexture; }

	// Events.
	virtual void OnTextureChange() {}
	virtual void OnAttributeChange() {}
	virtual void RebuildTexture() {}
};

// An object that can have integrity checked.
#define CHECK_OK	0x00000000
class _T_EXPORT CBaseCheckableObject
{
public:
	
	CBaseCheckableObject() {}
	virtual ~CBaseCheckableObject() {}

	int CheckObject() { return CHECK_OK; }
	CString GetCheckMessage(int nCode) { return "CBaseCheckable: No Error Message Defined."; }
};

// A VisgroupRef is an object that references a visgroup.
class _T_EXPORT CVisgroup;
class _T_EXPORT CVisgroupRef : public CLLObject
{
private:

	CVisgroup* m_pVisgroup;
	CString m_sName, m_sCategory;
	CTread3DDoc* m_pDocument;

public:

	CVisgroupRef();
	CVisgroupRef(const CVisgroupRef& VisgroupRefToCopy);
	virtual ~CVisgroupRef();
	
	bool Read(CFile& hFile);
	bool Write(CFile& hFile);

	void SetVisgroup(CVisgroup* pVisgroup) { m_pVisgroup = pVisgroup; }
	void SetVisgroup(CString sName, CString sCategory);
	CString GetName();
	CString GetCategory();
	bool IsVisible();
	void BindToDocument(CTread3DDoc* pDocument);
};

class _T_EXPORT CVisgroupRefObject
{
private:

	CVisgroupRef m_VisgroupRef;

public:

	CVisgroupRefObject() {}
	virtual ~CVisgroupRefObject() {}

	bool Read(CFile& hFile) { return m_VisgroupRef.Read(hFile); }
	bool Write(CFile& hFile) { return m_VisgroupRef.Write(hFile); }

	void SetVisgroup(CVisgroup* pVisgroup) { m_VisgroupRef.SetVisgroup(pVisgroup); }
	void SetVisgroup(CString sName, CString sCategory) { m_VisgroupRef.SetVisgroup(sName, sCategory); }
	CString GetName() { return m_VisgroupRef.GetName(); }
	CString GetCategory() { return m_VisgroupRef.GetCategory(); }
	bool IsVisible() { return m_VisgroupRef.IsVisible(); }
	void BindToDocument(CTread3DDoc* pDocument) { m_VisgroupRef.BindToDocument(pDocument); }
};

// A group of map objects.
class _T_EXPORT CObjectGroup : public CBaseGroupableObject, public CVisgroupRefObject, 
	public CBaseTransformableObject, public CBaseTextureableObject,
	public CBaseCarveableObject
{
protected:


	CLinkedList<CBaseObject>* m_pObjectList;

public:

	CObjectGroup();
	CObjectGroup(const CObjectGroup& ObjectToCopy);
	virtual ~CObjectGroup();

	// Adds/Removes an object.
	virtual bool AddObject(CBaseGroupableObject* pObject);
	virtual bool RemoveObject(CBaseGroupableObject* pObject);

	CLinkedList<CBaseObject>* GetObjectList() { return m_pObjectList; }

	// From CBaseObject.
	virtual int GetObjectClass() { return OBJECT_CLASS_GROUP; }
	virtual int GetObjectInterfaces() { return I_CARVE | I_CARVEABLE | I_GROUPABLE | I_GROUP | I_VISGROUPREF | I_TRANSFORMABLE | I_TEXTUREABLE | I_STORABLE; }
	virtual void* GetInterface(int nInterface);

	virtual void SetOrigin(vec3_t vOrigin);
	virtual void Scale(vec3_t vScale);
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual void ComputeOrigin();
	virtual void BuildRange();
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin);
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView);

	virtual CObjectGroup* CSGSubtract(CBaseObject* pObject);
	virtual bool ObjectsIntersect(CBaseObject* pObject);
	virtual int GetCarveClasses() const { return OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP; }

	virtual CBaseObject* Clone() { return new CObjectGroup(*this); }

	// Events.
	virtual void OnTextureChange();

	// Storable Object.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);
	virtual int InitAfterLoad(CTread3DDoc* pDocument);

	static CObjectGroup* CarveWithGroup(CBaseCarveableObject* pCarveTarget, CObjectGroup* pCarveGroup);
};
// $eb
CBaseObject* NewBaseObject(int nClassType);

#endif