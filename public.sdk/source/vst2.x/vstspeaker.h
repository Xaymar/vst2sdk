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

enum VstSpeakerArrangmentType { // Name adapted from vst.h
	kSpeakerArrUserDefined = -2,
	kSpeakerArr__unk01     = -1,
	// Taken from: VstInt32 Vst2Wrapper::vst3ToVst2SpeakerArr
	kSpeakerArrMono = 0,
	kSpeakerArrStereo,
	kSpeakerArrStereoSurround,
	kSpeakerArrStereoCenter,
	kSpeakerArrStereoSide,
	kSpeakerArrStereoCLfe,
	kSpeakerArr30Cine,
	kSpeakerArr30Music,
	kSpeakerArr31Cine,
	kSpeakerArr31Music,
	kSpeakerArr40Cine,
	kSpeakerArr40Music,
	kSpeakerArr41Cine,
	kSpeakerArr41Music,
	kSpeakerArr50,
	kSpeakerArr51,
	kSpeakerArr60Cine,
	kSpeakerArr60Music,
	kSpeakerArr61Cine,
	kSpeakerArr61Music,
	kSpeakerArr70Cine,
	kSpeakerArr70Music,
	kSpeakerArr71Cine,
	kSpeakerArr71Music,
	kSpeakerArr80Cine,
	kSpeakerArr80Music,
	kSpeakerArr81Cine,
	kSpeakerArr81Music,
	kSpeakerArr102,
};

enum VstSpeakerType { // Name adapted from vst.h
	// Taken from VstInt32 Vst2Wrapper::vst3ToVst2Speaker (Vst::Speaker vst3Speaker)
	kSpeakerUndefined = INT32_MAX, // Weirdest one to figure out. Why not -1?
	kSpeakerM         = 0,
	kSpeakerL,
	kSpeakerR,
	kSpeakerC,
	kSpeakerLfe,
	kSpeakerLs,
	kSpeakerRs,
	kSpeakerLc,
	kSpeakerRc,
	kSpeakerS,
	kSpeakerSl,
	kSpeakerSr,
	kSpeakerTm,
	kSpeakerTfl,
	kSpeakerTfc,
	kSpeakerTfr,
	kSpeakerTrl,
	kSpeakerTrc,
	kSpeakerTrr,
	kSpeakerLfe2,
};

struct VstSpeakerProperties {
	float    __unk00;
	float    __unk01;
	float    __unk02;
	float    __unk03;
	char     name[64];
	VstInt32 type; // VstSpeakerType, see vst.h
	char     __unk04[28];
};

struct VstSpeakerArrangement {
	VstInt32             type; // VstSpeakerArrangmentType, see vst.h
	VstInt32             numChannels;
	VstSpeakerProperties speakers[32]; // See vst.h. I don't know if this size is correct.
};

#pragma pack(pop)
