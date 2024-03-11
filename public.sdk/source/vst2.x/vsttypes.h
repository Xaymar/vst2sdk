/*
 * Copyright 2024 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
 * Copyright 2024 Steinberg Media Technologies GmbH
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#ifdef __cplusplus
#include <cinttypes>
#else
#include <inttypes.h>
#endif

#define VstFunctionAPI __cdecl

enum VstMaxLengths { // Anything with k is an enum?
	kVstMaxNameLen        = 64,
	kVstMaxLabelLen       = 64,
	kVstMaxShortLabelLen  = 8,
	kVstExtMaxParamStrLen = 32, // Apparently incorrect, but actually in use by many hosts.
	kVstMaxParamStrLen    = 8,
	kVstMaxProgNameLen    = 24,
	kVstMaxVendorStrLen   = 64,
	kVstMaxEffectNameLen  = 32,
};

// 16-bit wide Integer
typedef int16_t VstInt16;

// 32-bit wide Integer
typedef int32_t VstInt32;

// Variable size Integer Pointer
typedef intptr_t VstIntPtr;

// Float and double are used as-is. No custom type name here.

// Bus Direction. Appears to be int32_t
typedef int32_t BusDirection;

// Rectangle
struct ERect {
	VstInt16 left, top, right, bottom;
};
