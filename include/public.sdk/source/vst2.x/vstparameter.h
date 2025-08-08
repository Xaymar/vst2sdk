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
#include "vsttypes.h"

#pragma pack(push, 8)

enum VstParameterFlags {
	kVstParameterIsSwitch          = 1,
	kVstParameterUsesIntegerMinMax = 2,
	kVstParameterCanRamp           = 7,
};

struct VstParameterProperties {
	float    __unk00;
	float    __unk01;
	float    __unk02;
	char     label[64];
	VstInt32 flags;
	VstInt32 minInteger;
	VstInt32 maxInteger;
	VstInt32 __unk03;
	VstInt32 __unk04;
	char     shortLabel[8];
	// There's more after this, but it's not used?
	// We know what they're for (see vst.h), but they don't appear anywhere.
	VstInt16 __unk05; // Order/Index for display. Starts at 0.
	VstInt16 __unk06; // Category Index, 0 for none, 1+ for category.
	VstInt16 __unk07; // Number of parameters in this category
	VstInt16 __unk08; // Know absolutely nothing about this one.
	char     __unk09[24]; // Category Label. We know the size here, and what it is.
	char     __unk10[16]; // Always exists, not sure why. May be padding.
};

#pragma pack(pop)
