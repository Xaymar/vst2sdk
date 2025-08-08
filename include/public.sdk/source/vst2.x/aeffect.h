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

enum AEffectOpCodes {
	audioEffectOpen, // Create/Open Effect
	audioEffectClose, // Destroy/Close Effect
	audioEffectSetProgram, // Set Program
	audioEffectGetProgram, // Get Program
	audioEffectSetProgramName, // Set Program Name
	audioEffectGetProgramName, // Get Program Name
	audioEffectGetParameterLabel, // Get Parameter Label
	audioEffectGetParameterDisplay, // Get Parameter Display
	audioEffectGetParameterName, // Get Parameter Name
	AEffectOpCode_09,
	audioEffectSetSampleRate, // Set Sample Rate
	audioEffectSetBlockSize, // Set Block Size
	audioEffectSuspendResume, // Suspend/Resume
	audioEffectEditorGetSize, // Get Editor Size
	audioEffectEditorOpen, // Create/Open Editor
	audioEffectEditorClose, // Destroy/Close Editor
	AEffectOpCode_10,
	AEffectOpCode_11,
	AEffectOpCode_12,
	audioEffectEditorDoNothing,
	AEffectOpCode_14,
	AEffectOpCode_15,
	AEffectOpCode_16, // Always FOURCC('N', 'v', 'E', 'F')? Might be different for some VST 1.x and 2.x plug-ins. Doesn't seem to be used.
	audioEffectGetChunk, // Get Chunk
	audioEffectSetChunk, // Set Chunk

	// All OpCodes above this maximum are version 2.x
	AEffectOpCodeMax
};

enum AEffectMasterOpCodes {
	AEffectMasterOpCode_00,
	audioMasterAutomate = AEffectMasterOpCode_00,
	AEffectMasterOpCode_01,
	audioMasterVersion = AEffectMasterOpCode_01,
	AEffectMasterOpCode_02,
	AEffectMasterOpCode_03,
	AEffectMasterOpCode_04,

	// All OpCodes above this maximum are version 2.x
	AEffectMasterOpCodeMax,
};

enum AEffectFlags { // Guessed name based on struct+field combination.
	AEffectFlagHasEditor         = 1 << 0,
	AEffectFlagCanReplacing      = 1 << 4,
	AEffectFlagProgramsAreChunks = 1 << 5, // Not sure what this means.
	AEffectFlagIsSynthesizer     = 1 << 8,
	AEffectFlagIsSilentOnSilence = 1 << 9, // Does not generate tail samples?
};

struct AEffect {
	VstInt32 magic; // Must be FOURCC('V', 's', 't', 'P')

	VstIntPtr(VstFunctionAPI* control)(AEffect* self, VstInt32 opcode, VstInt32 param1, VstIntPtr param2, void* ptr, float value);

#if (!defined VST_VERSION_SUPPORT) || (VST_VERSION_SUPPORT >= kVstVersion_2400)
	// No 2.4 host ever calls this, processReplacing and processDoubleReplacing seem to be required.
	[[deprecated]]
#endif
	void(VstFunctionAPI* process)(AEffect* self, float** inputs, float** outputs, VstInt32 sampleFrames);

	void(VstFunctionAPI* setParameter)(AEffect* self, VstInt32 index, float value);

	float(VstFunctionAPI* getParameter)(AEffect* self, VstInt32 index);

	VstInt32 numPrograms;
	VstInt32 numParams;
	VstInt32 numInputs; // Guessing names here based on the other ones. We already know what their type is anyway.
	VstInt32 numOutputs;
	VstInt32 flags; // AEffectFlags, see vst.h for more.

	VstIntPtr __unk04;
	VstIntPtr __unk05;

	VstInt32 delay;

	VstInt32 __unk06;
	VstInt32 __unk07;
	float    __unk08;

	void* effect_data; // I think this is effect private data.
	void* host_data; // And this is host private data.

	VstInt32 uniqueID; // A very collidable unique id. 32 bit is not a lot.
	VstInt32 version; // VstVersion

	/** Process float data in-place.
	 *
	 * Must be present for 2.4 or later hosts. Make sure you set the proper flag.
	 * 
	 * \param inputs Input buffers. Can have pointers used in outputs.
	 * \param outputs Output buffers. Can have pointers used in inputs.
	 * \param sampleFrames Number of samples in all buffers.
	 */
	void(VstFunctionAPI* processReplacing)(AEffect* self, float** inputs, float** outputs, VstInt32 sampleFrames);

#if (!defined VST_VERSION_SUPPORT) || (VST_VERSION_SUPPORT >= kVstVersion_2400)
	// These only appear in version >= 2.4

	/** Process double data in-place.
	 *
	 * \param inputs Input buffers. Can have pointers used in outputs.
	 * \param outputs Output buffers. Can have pointers used in inputs.
	 * \param sampleFrames Number of samples in all buffers.
	 */
	void(VstFunctionAPI* processDoubleReplacing)(AEffect* self, double** inputs, double** outputs, VstInt32 sampleFrames);

	char __unk12[56];
#else
	char __unk12[60];
#endif
};

// Master callback.
typedef VstIntPtr(VstFunctionAPI* audioMasterCallback)(AEffect*, VstInt32 opcode, VstInt32, VstInt32, void* ptr, float);

#pragma pack(pop)
