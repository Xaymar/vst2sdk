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
#include "vstversion.h"

#pragma pack(push, 8)

enum VstMidiEventFlags {
	kVstMidiEventIsRealtime,
};

struct VstMidiEvent : VstEvent {
	VstInt32 noteLength;
	VstInt32 noteOffset;
	char     midiData[4];
	char     detune;
	char     noteOffVelocity;
	char     __unk00;
	char     __unk01;
};

struct VstMidiSysexEvent : VstEvent {
	VstInt32  dumpBytes;
	VstIntPtr __unk00;
	char*     sysexDump;
	VstIntPtr __unk01;
};

struct MidiKeyName {
	VstInt32 thisProgramIndex;
	VstInt32 thisKeyNumber;
	char     keyName[kVstMaxNameLen];
	VstInt32 __unk00;
	VstInt32 __unk01;
};

struct MidiProgramName {
	VstInt32 thisProgramIndex;
	char     name[kVstMaxNameLen];
	VstInt32 midiProgram; // Could be swapped with thisProgramIndex?
	VstInt32 midiBankMsb;
	VstInt32 midiBankLsb;
	VstInt32 __unk01;
	VstInt32 parentCategoryIndex; // -1 appears to mean no category.
	VstInt32 flags;
};

struct MidiProgramCategory {
	VstInt32 thisCategoryIndex;
	char     name[kVstMaxNameLen]; // Name guessed, but might be wrong. Doesn't seem to be used.
	VstInt32 parentCategoryIndex;
	VstInt32 flags;
};

#pragma pack(pop)
