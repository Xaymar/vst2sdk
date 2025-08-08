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

enum VstSMPTEFrameRate {
	// Taken from: void Vst2Wrapper::setupProcessTimeInfo ()
	kVstSmpte24fps    = 0,
	kVstSmpte25fps    = 1,
	kVstSmpte2997fps  = 2,
	kVstSmpte30fps    = 3,
	kVstSmpte2997dfps = 4,
	kVstSmpte30dfps   = 5,
	kVstSmpteFilm16mm = 6,
	kVstSmpteFilm35mm = 7,
	kVstSmpte239fps   = 9,
	kVstSmpte249fps   = 10,
	kVstSmpte599fps   = 11,
	kVstSmpte60fps    = 12,
};

struct VstTimeInfo {
	double   samplePos;
	double   sampleRate;
	double   nanoSeconds;
	double   ppqPos;
	double   tempo;
	double   barStartPos;
	double   cycleStartPos;
	double   cycleEndPos;
	VstInt32 timeSigNumerator;
	VstInt32 timeSigDenominator;
	VstInt32 smpteOffset;
	VstInt32 smpteFrameRate;
	VstInt32 samplesToNextClock;
	VstInt32 flags;
};

#pragma pack(pop)
