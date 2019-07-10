
//feature flags
#define FPU			0x00000001
#define VME			0x00000002
#define DBEXT		0x00000004
#define PSE			0x00000008
#define TSC			0x00000010
#define MSR			0x00000020
#define PAE			0x00000040
#define MCE			0x00000080
#define CMP8		0x00000100
#define APIC		0x00000200		
#define NDEF1		0x00000400		//Undefined
#define SYS			0x00000800
#define MTRR		0x00001000
#define PGE			0x00002000
#define MCA			0x00004000
#define CMOV		0x00008000
#define PAT			0x00010000
#define PSE36		0x00020000
#define PSN			0x00040000
#define NDEF2		0x00080000		//Undefined
#define NDEF3		0x00100000		//Undefined
#define NDEF4		0x00200000		//Undefined
#define NDEF5		0x00400000		//Undefined
#define MMX			0x00800000		//MMX instructions
#define FSR			0x01000000
#define SIMD		0x02000000
#define NDEF6		0x04000000		//Undefined
#define NDEF7		0x08000000		//Undefined
#define NDEF8		0x10000000		//Undefined
#define NDEF9		0x20000000		//Undefined
#define NDEF10		0x40000000		//Undefined
#define AMD_3DNOW	0x80000000		//3DNow! Instructions

struct CPU_INFO
{
	char szVendor[13];
	unsigned int dwStdLevel;
	unsigned int dwVersion;
	unsigned int dwReserved1;
	unsigned int dwReserved2;
	unsigned int dwFeatures;
	unsigned int dwCacheTLB1;
	unsigned int dwCacheTLB2;
	unsigned int dwCacheTLB3;
	unsigned int dwCacheTLB4;
	unsigned int dwExtCount;
	unsigned int* pExtended[4];

	CPU_INFO();
	~CPU_INFO();
};


bool IsCpuidPresent();

unsigned int GetCpuSpeed(void);

void GetCpuInfo(CPU_INFO &cpu);
int GetCpuFamily(const CPU_INFO &cpu);
int GetCpuModel(const CPU_INFO &cpu);
int GetCpuStepping(const CPU_INFO &cpu);
int GetCpuType(const CPU_INFO &cpu);
CString GetCpuVendor(const CPU_INFO &cpu);
bool GetCpuFeature(const CPU_INFO &cpu, unsigned int nFeature);
CString FeatureDescription(unsigned int nFeature);
CString GetCpuString(const CPU_INFO &cpu);
