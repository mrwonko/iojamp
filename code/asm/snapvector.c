/*
===========================================================================
Copyright (C) 2011 Thilo Schulz <thilo@tjps.eu>

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "qasm-inline.h"
#include "../qcommon/q_shared.h"

#ifndef _MSC_VER

/*
 * GNU inline asm version of qsnapvector
 * See MASM snapvector.asm for commentary
 */

static unsigned char ssemask[16] __attribute__((aligned(16))) =
{
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\x00\x00\x00"
};

void qsnapvectorsse(vec3_t vec)
{
	__asm__ volatile
	(
		"movaps (%0), %%xmm1\n"
		"movups (%1), %%xmm0\n"
		"movaps %%xmm0, %%xmm2\n"
		"andps %%xmm1, %%xmm0\n"
		"andnps %%xmm2, %%xmm1\n"
		"cvtps2dq %%xmm0, %%xmm0\n"
		"cvtdq2ps %%xmm0, %%xmm0\n"
		"orps %%xmm1, %%xmm0\n"
		"movups %%xmm0, (%1)\n"
		:
		: "r" (ssemask), "r" (vec)
		: "memory", "%xmm0", "%xmm1", "%xmm2"
	);
	
}

#define QROUNDX87(src) \
	"flds " src "\n" \
	"fistpl " src "\n" \
	"fildl " src "\n" \
	"fstps " src "\n"	

void qsnapvectorx87(vec3_t vec)
{
	__asm__ volatile
	(
		QROUNDX87("(%0)")
		QROUNDX87("4(%0)")
		QROUNDX87("8(%0)")
		:
		: "r" (vec)
		: "memory"
	);
}

#else

/* MSVC Versions of the above */

static unsigned char __declspec(align(16)) ssemask[16] =
{
	'\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF',0,0,0,0
};

void __declspec(naked) qsnapvectorsse(vec3_t vec)
{
	__asm
	{
#if idx64
		sub rsp, 8
		movaps xmm1, ssemask		; initialize the mask register
		movups xmm0, [rcx]			; here is stored our vector. Read 4 values in one go
		movaps xmm2, xmm0			; keep a copy of the original data
		andps xmm0, xmm1			; set the fourth value to zero in xmm0
		andnps xmm1, xmm2			; copy fourth value to xmm1 and set rest to zero
		cvtps2dq xmm0, xmm0			; convert 4 single fp to int
		cvtdq2ps xmm0, xmm0			; convert 4 int to single fp
		orps xmm0, xmm1				; combine all 4 values again
		movups [rcx], xmm0			; write 3 rounded and 1 unchanged values back to memory
		ret
#else
		mov eax, dword ptr 4[esp]	; store address of vector in eax
		movaps xmm1, ssemask		; initialize the mask register for maskmovdqu
		movups xmm0, [eax]			; here is stored our vector. Read 4 values in one go
		movaps xmm2, xmm0			; keep a copy of the original data
		andps xmm0, xmm1			; set the fourth value to zero in xmm0
		andnps xmm1, xmm2			; copy fourth value to xmm1 and set rest to zero
		cvtps2dq xmm0, xmm0			; convert 4 single fp to int
		cvtdq2ps xmm0, xmm0			; convert 4 int to single fp
		orps xmm0, xmm1				; combine all 4 values again
		movups [eax], xmm0			; write 3 rounded and 1 unchanged values back to memory
		ret
#endif
	}
}

#if !idx64

void __declspec(naked) qsnapvectorx87(vec3_t vec)
{
	__asm
	{
		mov eax, dword ptr 4[esp]

		fld dword ptr [eax]
		fistp dword ptr [eax]
		fild dword ptr [eax]
		fstp dword ptr [eax]

		fld dword ptr 4[eax]
		fistp dword ptr 4[eax]
		fild dword ptr 4[eax]
		fstp dword ptr 4[eax]

		fld dword ptr 8[eax]
		fistp dword ptr 8[eax]
		fild dword ptr 8[eax]
		fstp dword ptr 8[eax]

		ret
	}
}
#endif

#endif
