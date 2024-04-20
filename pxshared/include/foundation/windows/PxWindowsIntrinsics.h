// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2023 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.

#ifndef PXFOUNDATION_PXWINDOWSINTRINSICS_H
#define PXFOUNDATION_PXWINDOWSINTRINSICS_H

#include "foundation/Px.h"
#include "foundation/PxSharedAssert.h"

// this file is for internal intrinsics - that is, intrinsics that are used in
// cross platform code but do not appear in the API

#if !PX_WINDOWS_FAMILY
#error "This file should only be included by Windows builds!!"
#endif

#pragma warning(push)
//'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
#pragma warning(disable : 4668)
#if PX_VC == 10
#pragma warning(disable : 4987) // nonstandard extension used: 'throw (...)'
#endif
#include <intrin.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4985) // 'symbol name': attributes not present on previous declaration
#include <math.h>
#pragma warning(pop)

#include <float.h>
// do not include for ARM target
#if !PX_ARM && !PX_A64
#include <mmintrin.h>
#endif

#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)

#if !PX_DOXYGEN
namespace physx
{
namespace intrinsics
{
#endif

/*
* Implements a memory barrier
*/
PX_FORCE_INLINE void PxMemoryBarrier()
{
	_ReadWriteBarrier();
	/* long Barrier;
	__asm {
	    xchg Barrier, eax
	}*/
}

//! \brief platform-specific absolute value
        PX_CUDA_CALLABLE PX_FORCE_INLINE float abs(float a)
        {
            return ::fabsf(a);
        }

//! \brief platform-specific select float
        PX_CUDA_CALLABLE PX_FORCE_INLINE float fsel(float a, float b, float c)
        {
            return (a >= 0.0f) ? b : c;
        }

//! \brief platform-specific sign
        PX_CUDA_CALLABLE PX_FORCE_INLINE float sign(float a)
        {
            return (a >= 0.0f) ? 1.0f : -1.0f;
        }

//! \brief platform-specific reciprocal
        PX_CUDA_CALLABLE PX_FORCE_INLINE float recip(float a)
        {
            return 1.0f / a;
        }

//! \brief platform-specific reciprocal estimate
        PX_CUDA_CALLABLE PX_FORCE_INLINE float recipFast(float a)
        {
            return 1.0f / a;
        }

//! \brief platform-specific square root
        PX_CUDA_CALLABLE PX_FORCE_INLINE float sqrt(float a)
        {
            return ::sqrtf(a);
        }

//! \brief platform-specific reciprocal square root
        PX_CUDA_CALLABLE PX_FORCE_INLINE float recipSqrt(float a)
        {
            return 1.0f / ::sqrtf(a);
        }

//! \brief platform-specific reciprocal square root estimate
        PX_CUDA_CALLABLE PX_FORCE_INLINE float recipSqrtFast(float a)
        {
            return 1.0f / ::sqrtf(a);
        }

//! \brief platform-specific sine
        PX_CUDA_CALLABLE PX_FORCE_INLINE float sin(float a)
        {
            return ::sinf(a);
        }

//! \brief platform-specific cosine
        PX_CUDA_CALLABLE PX_FORCE_INLINE float cos(float a)
        {
            return ::cosf(a);
        }

//! \brief platform-specific minimum
        PX_CUDA_CALLABLE PX_FORCE_INLINE float selectMin(float a, float b)
        {
            return a < b ? a : b;
        }

//! \brief platform-specific maximum
        PX_CUDA_CALLABLE PX_FORCE_INLINE float selectMax(float a, float b)
        {
            return a > b ? a : b;
        }

//! \brief platform-specific finiteness check (not INF or NAN)
        PX_CUDA_CALLABLE PX_FORCE_INLINE bool isFinite(float a)
        {
#ifdef __CUDACC__
            return !!isfinite(a);
#else
            return (0 == ((_FPCLASS_SNAN | _FPCLASS_QNAN | _FPCLASS_NINF | _FPCLASS_PINF) & _fpclass(a)));
#endif
        }

//! \brief platform-specific finiteness check (not INF or NAN)
        PX_CUDA_CALLABLE PX_FORCE_INLINE bool isFinite(double a)
        {
#ifdef __CUDACC__
            return !!isfinite(a);
#else
            return (0 == ((_FPCLASS_SNAN | _FPCLASS_QNAN | _FPCLASS_NINF | _FPCLASS_PINF) & _fpclass(a)));
#endif
        }

/*!
Sets \c count bytes starting at \c dst to zero.
*/
        PX_FORCE_INLINE void* memZero(void* dest, uint32_t count)
        {
            return memset(dest, 0, count);
        }

/*!
Sets \c count bytes starting at \c dst to \c c.
*/
        PX_FORCE_INLINE void* memSet(void* dest, int32_t c, uint32_t count)
        {
            return memset(dest, c, count);
        }

/*!
Copies \c count bytes from \c src to \c dst. User memMove if regions overlap.
*/
        PX_FORCE_INLINE void* memCopy(void* dest, const void* src, uint32_t count)
        {
            return memcpy(dest, src, count);
        }

/*!
Copies \c count bytes from \c src to \c dst. Supports overlapping regions.
*/
        PX_FORCE_INLINE void* memMove(void* dest, const void* src, uint32_t count)
        {
            return memmove(dest, src, count);
        }

/*!
Returns the index of the highest set bit. Not valid for zero arg.
*/
PX_FORCE_INLINE uint32_t PxHighestSetBitUnsafe(uint32_t v)
{
	unsigned long retval;
	_BitScanReverse(&retval, v);
	return retval;
}

/*!
Returns the index of the highest set bit. Undefined for zero arg.
*/
PX_FORCE_INLINE uint32_t PxLowestSetBitUnsafe(uint32_t v)
{
	unsigned long retval;
	_BitScanForward(&retval, v);
	return retval;
}

/*!
Returns the number of leading zeros in v. Returns 32 for v=0.
*/
PX_FORCE_INLINE uint32_t PxCountLeadingZeros(uint32_t v)
{
	if(v)
	{
		unsigned long bsr = (unsigned long)-1;
		_BitScanReverse(&bsr, v);
		return 31 - bsr;
	}
	else
		return 32;
}

/*!
Prefetch aligned cache size around \c ptr+offset.
*/
#if !PX_ARM && !PX_A64
PX_FORCE_INLINE void PxPrefetchLine(const void* ptr, uint32_t offset = 0)
{
	// cache line on X86/X64 is 64-bytes so a 128-byte prefetch would require 2 prefetches.
	// However, we can only dispatch a limited number of prefetch instructions so we opt to prefetch just 1 cache line
	/*_mm_prefetch(((const char*)ptr + offset), _MM_HINT_T0);*/
	// We get slightly better performance prefetching to non-temporal addresses instead of all cache levels
	_mm_prefetch(((const char*)ptr + offset), _MM_HINT_NTA);
}
#else
PX_FORCE_INLINE void PxPrefetchLine(const void* ptr, uint32_t offset = 0)
{
	// arm does have 32b cache line size
	__prefetch(((const char*)ptr + offset));
}
#endif

/*!
Prefetch \c count bytes starting at \c ptr.
*/
#if !PX_ARM
PX_FORCE_INLINE void PxPrefetch(const void* ptr, uint32_t count = 1)
{
	const char* cp = (char*)ptr;
	uint64_t p = size_t(ptr);
	uint64_t startLine = p >> 6, endLine = (p + count - 1) >> 6;
	uint64_t lines = endLine - startLine + 1;
	do
	{
		PxPrefetchLine(cp);
		cp += 64;
	} while(--lines);
}
#else
PX_FORCE_INLINE void PxPrefetch(const void* ptr, uint32_t count = 1)
{
	const char* cp = (char*)ptr;
	uint32_t p = size_t(ptr);
	uint32_t startLine = p >> 5, endLine = (p + count - 1) >> 5;
	uint32_t lines = endLine - startLine + 1;
	do
	{
		PxPrefetchLine(cp);
		cp += 32;
	} while(--lines);
}
#endif

#if !PX_DOXYGEN
    } // namespace intrinsics
} // namespace physx
#endif

#endif

