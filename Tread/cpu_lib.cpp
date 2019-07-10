#include "stdafx.h"
#include <mmsystem.h>
#include <math.h>
#include "cpu_lib.h"

#pragma warning( disable : 4035 )

CPU_INFO::CPU_INFO()
{
	*pExtended = NULL;
}

CPU_INFO::~CPU_INFO()
{
	if(pExtended != NULL)
	{
		delete [] *pExtended;
		*pExtended = NULL;
	}
}

unsigned int GetCpuSpeed(void)
{
	int				timeStart	= 0;
	int				timeStop	= 0;
	unsigned long   StartTicks	= 0;
	unsigned long   EndTicks	= 0;
	unsigned long   TotalTicks	= 0;
	unsigned long   cpuSpeed	= 0;


		timeStart = timeGetTime();				//GET TICK EDGE
		for(;;)
		{
			timeStop = timeGetTime();		
			if ( (timeStop-timeStart) > 1 )		// ROLLOVER PAST 1
			{
				__asm{
					 xor    eax, eax
					 xor    ebx, ebx
					 xor    ecx, ecx
					 xor    edx, edx
					 _emit  0x0f				// CPUID
					 _emit  0xa2
					 _emit  0x0f				// RDTSC
					 _emit  0x31
					 mov    [StartTicks], eax
					}
				break;	
			}	
		}

		timeStart = timeStop;				

		for(;;)
		{
			timeStop = timeGetTime();			
			if ( (timeStop-timeStart) > 1000 )	//ONE SECOND
			{
				__asm{
					 xor    eax, eax
					 xor    ebx, ebx
					 xor    ecx, ecx
					 xor    edx, edx
					 _emit  0x0f				// CPUID
					 _emit  0xa2
					 _emit  0x0f				// RDTSC
					 _emit  0x31
					 mov    [EndTicks], eax
					}

				break;	
			}	
		}

		TotalTicks = EndTicks-StartTicks;		// TOTAL 

		cpuSpeed = TotalTicks/1000000;			// SPEED

	return (UINT)cpuSpeed;			
}

bool IsCpuidPresent()
{
	_asm
	{
		pushfd								; Save EFLAGS to stack
		pop eax								; Store EFLAGS in EAX
		mov ebx, eax						; Save in EBX for testing later
		xor eax, 00200000h					; Switch bit 21
		push eax							; Copy changed value to stack
		popfd								; Save changed EAX to EFLAGS
		pushfd								; Push EFLAGS to top of stack
		pop eax								; Store EFLAGS in EAX
		cmp eax, ebx						; See if bit 21 has changed
		jne True							; If no change, no CPUID
		mov eax, 0							;return false
		jmp END								;jump to the end
True:
		mov eax, 1							;return true
END:
	}
}

void GetCpuInfo(CPU_INFO &cpu)
{
	_asm
	{
		mov edi, cpu						;copy address of cpu
		mov eax, 0							;function 1
		cpuid								;cpuid			
		mov [edi].dwStdLevel, eax			;save the MaxLevels
		lea esi, [edi].szVendor				;load the address of the vendor string
		mov dword ptr [esi], ebx			;move vendor string
		mov dword ptr [esi + 4], edx		;move vendor string
		mov dword ptr [esi + 8], ecx		;move vendor string
		mov byte ptr [esi + 12], 0			;null terminate the string
		
		mov eax, 1							;function 2
		cpuid								;cpuid
		mov [edi]CPU_INFO.dwVersion, eax			;save the version info
		mov [edi]CPU_INFO.dwReserved1, ebx	;no information
		mov [edi]CPU_INFO.dwReserved2, ecx	;no information
		mov [edi]CPU_INFO.dwFeatures, edx			;save the feature flags

		cmp [edi].dwStdLevel, 2				;max input for cpuid
		jnae END							;if not above or equal
		mov eax, 2							;function 3
		cpuid								;cpuid
		mov [edi]CPU_INFO.dwCacheTLB1, eax	;save the tlb and cache info
		mov [edi]CPU_INFO.dwCacheTLB2, ebx	;save the tlb and cache info
		mov [edi]CPU_INFO.dwCacheTLB3, ecx	;save the tlb and cache info
		mov [edi]CPU_INFO.dwCacheTLB4, edx	;save the tlb and cache info

		;Get extende information if it is available
		mov eax, 0x80000000      // Check for support of extended functions.
		mov [edi]CPU_INFO.dwExtCount, eax
		cmp eax, 1
		jb END
	}

	*cpu.pExtended = new unsigned int[cpu.dwExtCount];

	_asm
	{
		cmp [edi]CPU_INFO.pExtended, 0
		je END
		mov ecx, [edi]CPU_INFO.dwExtCount
END:
	}
}

int GetCpuFamily(const CPU_INFO &cpu)
{
	_asm
	{
		mov ebx, cpu				;load the address of the structure
		mov eax, [ebx]CPU_INFO.dwVersion	;move the value to the register
		shr eax, 8					;move to the right 8 bit positions
		and eax, 0xF				;clear unwanted bits
	}
}

int GetCpuModel(const CPU_INFO &cpu)
{
	_asm
	{
		mov ebx, cpu				;load the address of the structure
		mov eax, [ebx]CPU_INFO.dwVersion	;move the value to the register
		shr eax, 4					;move to the right 4 bit positions
		and eax, 0xF				;clear unwanted bits
	}
}

int GetCpuStepping(const CPU_INFO &cpu)
{
	_asm
	{
		mov ebx, cpu				;load the address of the structure
		mov eax, [ebx]CPU_INFO.dwVersion	;move the value to the register
		and eax, 0xF				;clear unwanted bits
	}
}

int GetCpuType(const CPU_INFO &cpu)
{
	_asm
	{
		mov ebx, cpu				;load the address of the structure
		mov eax, [ebx]CPU_INFO.dwVersion	;move the value to the register
		shr eax, 12					;move to the right 4 bit positions
		and eax, 0xF				;clear unwanted bits
	}
}

CString GetCpuVendor(const CPU_INFO &cpu)
{
	CString sBuffer;

	for(int n = 0; n < 12; n++)
		sBuffer.SetAt(n, cpu.szVendor[n]);

	return sBuffer;
}

bool GetCpuFeature(const CPU_INFO &cpu, unsigned int nFeature)
{
	if(cpu.dwFeatures & nFeature)
		return true;
	else
		return false;
}

CString FeatureDescription(unsigned int nFeature)
{
	CString sBuffer = "";
	
	if(nFeature > 31)
		return sBuffer;

	switch(nFeature)
	{
	case FPU:
		sBuffer = "Floating Point Processor";
		break;
	case VME:
		sBuffer = "Virtual Mode Extensions";
		break;
	case DBEXT:
		sBuffer = "Debugging Extension";
		break;
	case PSE:
		sBuffer = "Page Size Extension";
		break;
	case TSC:
		sBuffer = "Time Stamp Counter";
		break;
	case MSR:
		sBuffer = "RDMSR / WRMSR Instruction";
		break;
	case PAE:
		sBuffer = "Physical Address Extension";
		break;
	case MCE:
		sBuffer = "Machine Check Exception";
		break;
	case CMP8:
		sBuffer = "CMPXCHG8B Instruction";
		break;
	case APIC:
		sBuffer = "On-Chip APIC Hardware";
		break;
	case SYS:
		sBuffer = "SYSENTER / SYSEXIT Instruction";
		break;
	case MTRR:
		sBuffer = "Memory Type Range Registers";
		break;
	case PGE:
		sBuffer = "Page Global Enable";
		break;
	case MCA:
		sBuffer = "Machine Check Architecture";
		break;
	case CMOV:
		sBuffer = "Conditional Move Instruction";
		break;
	case PAT:
		sBuffer = "Page Attribute Table";
		break;
	case PSE36:
		sBuffer = "36-Bit Page Size Extension";
		break;
	case PSN:
		sBuffer = "Processor Serial Number";
		break;
	case MMX:
		sBuffer = "MMX Instructions";
		break;
	case FSR:
		sBuffer = "Fast FPU Save and Restore";
		break;
	case SIMD:
		sBuffer = "Streaming SIMD Extensions";
		break;
	case AMD_3DNOW:
		sBuffer = "3DNow!";
		break;
	default:
		sBuffer = "Undefined";
	}

	return sBuffer;
}

CString GetCpuString(const CPU_INFO &cpu)
{
	int nFamily = GetCpuFamily(cpu);
	int nModel = GetCpuModel(cpu);
	CString sCpu = cpu.szVendor;
	CString sBuffer;

	/*
	switch(nFamily)
	{
	case 0: sBuffer = ""; break;
	case 1: sBuffer = ""; break;
	case 2: sBuffer = ""; break;
	case 3: sBuffer = ""; break;
	case 4: sBuffer = ""; break;
	case 5: sBuffer = ""; break;
	case 6: sBuffer = ""; break;
	case 7: sBuffer = ""; break;
	case 8: sBuffer = ""; break;
	case 9: sBuffer = ""; break;
	default:
		sBuffer = "Unknown";
	}
	*/

	if(!sCpu.CompareNoCase("GenuineIntel"))
	{
		switch(nFamily)
		{
		case 4:
			switch(nModel)
			{
			case 0:
			case 1:	sBuffer = "Intel 486 DX";	break;
			case 2:	sBuffer = "Intel 486 SX";	break;
			case 3:	sBuffer = "Intel 486 DX2";	break;
			case 4:	sBuffer = "Intel 486 SL";	break;
			case 5:	sBuffer = "Intel SX2";	break;
			case 7:	sBuffer = "Intel 486 DX2E";	break;
			case 8:	sBuffer = "Intel 486 DX4";	break;
			default:	sBuffer = "Unknown";
			}
			break;
		case 5:
			switch(nModel)
			{
			case 1:
			case 2:
			case 3: sBuffer = "Intel Pentium"; break;
			case 4: sBuffer = "Intel Pentium MMX"; break;
			default:
				sBuffer = "Unknown";
			}
			break;
		case 6:
			switch(nModel)
			{
			case 1: sBuffer = "Intel Pentium Pro"; break;
			case 3:
			case 5: sBuffer = "Intel Pentum II"; break;
			case 6: sBuffer = "Intel Celeron"; break;
			case 7: sBuffer = "Intel Pentium III"; break;
			default:
				sBuffer = "Unknown";
			}
			break;
		default:
			sBuffer = "Unknown";
		}
	}
	else if(!sCpu.CompareNoCase("CyrixInstead"))
	{
		switch(nFamily)
		{
		case 4: sBuffer = "MediaGX"; break;
		case 5: 
			switch(nModel)
			{
			case 2: sBuffer = "6x86L"; break;
			case 4: sBuffer = "MediaGX MMX"; break;
			default:
				sBuffer = "Unknown";
			}
		case 6: sBuffer = "6x86 MII"; break;
		default:
			sBuffer = "Unknown";
		}
	}
	else if(!sCpu.CompareNoCase("AuthenticAmd"))
	{
		switch(nFamily)
		{
		case 4:	sBuffer = "Am 486 / AM5x86";	break;
		case 5:
			switch(nModel)
			{
			case 0:
			case 1:
			case 2:
			case 3:	sBuffer = "AMD K5";	break;
			case 4:
			case 7:	sBuffer = "AMD K6 MMX";	break;
			case 8:	sBuffer = "AMD K6-2";	break;
			case 9:	sBuffer = "AMD K6-3";	break;
			default:	sBuffer = "Unknown";
			}
			break;
		case 6:	sBuffer = "AMD K7";	break;
		default:	sBuffer = "Unknown";
		}
	}
	else if(!sCpu.CompareNoCase("CentaurHauls"))
	{
		switch(nFamily)
		{
		case 5: 
			switch(nModel)
			{
			case 4: sBuffer = "IDT WinChip C6"; break;
			case 8: sBuffer = "IDT WinChip 2"; break;
			default:
				sBuffer = "Unknown";
			}
		case 6: sBuffer = ""; break;
		case 7: sBuffer = ""; break;
		case 8: sBuffer = ""; break;
		case 9: sBuffer = ""; break;
		default:
			sBuffer = "Unknown";
		}
	}
	else
		sBuffer = "Unknown";

	return sBuffer;
}