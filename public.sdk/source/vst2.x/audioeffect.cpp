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

#include "audioeffect.hpp"
#include <cstring>
#include "aeffect.h"

#define FOURCC(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | d)

AudioEffect::AudioEffect(audioMasterCallback p_audioMaster, VstInt32 __unk00, VstInt32 __unk01)
{
	memset(&cEffect, 0, sizeof(AEffect));
	cEffect.magic       = FOURCC('V', 's', 't', 'P');
	cEffect.effect_data = this;
#if (!defined VST_VERSION_SUPPORT)
	cEffect.version = kVstVersion_2400;
#else
	cEffect.version = VST_VERSION_SUPPORT;
#endif
	cEffect.control          = [](AEffect* self, VstInt32 opcode, VstInt32 param1, VstIntPtr param2, void* ptr, float value) { return reinterpret_cast<AudioEffect*>(self->effect_data)->control(opcode, param1, param2, ptr, value); };
	cEffect.process          = [](AEffect* self, float** inputs, float** outputs, VstInt32 sampleFrames) { return reinterpret_cast<AudioEffect*>(self->effect_data)->process(inputs, outputs, sampleFrames); };
	cEffect.setParameter     = [](AEffect* self, VstInt32 index, float value) { reinterpret_cast<AudioEffect*>(self->effect_data)->setParameter(index, value); };
	cEffect.getParameter     = [](AEffect* self, VstInt32 index) { return reinterpret_cast<AudioEffect*>(self->effect_data)->getParameter(index); };
	cEffect.processReplacing = [](AEffect* self, float** inputs, float** outputs, VstInt32 sampleFrames) { return reinterpret_cast<AudioEffect*>(self->effect_data)->processReplacing(inputs, outputs, sampleFrames); };

	audioMaster = p_audioMaster;
	numPrograms = cEffect.numPrograms;
	curProgram  = 0;
	editor      = nullptr;
}

AudioEffect::~AudioEffect() {}

VstIntPtr AudioEffect::control(VstInt32 opcode, VstInt32 param1, VstIntPtr param2, void* ptr, float value)
{
	switch (opcode) {
	case audioEffectOpen:
		open();
		break;
	case audioEffectClose:
		close();
		break;
	case audioEffectSetProgram:
		setProgram(param1);
		break;
	case audioEffectGetProgram:
		return getProgram();
	case audioEffectSetProgramName:
		setProgramName((char*)ptr);
		break;
	case audioEffectGetProgramName:
		getProgramName((char*)ptr);
		break;
	case audioEffectGetParameterLabel:
		getParameterLabel(param1, (char*)ptr);
		break;
	case audioEffectGetParameterDisplay:
		getParameterDisplay(param1, (char*)ptr);
		break;
	case audioEffectGetParameterName:
		getParameterName(param1, (char*)ptr);
		break;
	case audioEffectSetSampleRate:
		setSampleRate(value);
		break;
	case audioEffectSetBlockSize:
		setBlockSize(static_cast<VstInt32>(value));
		break;
	case audioEffectSuspendResume:
		if (!param2) {
			suspend();
		} else {
			resume();
		}
		break;
	case audioEffectEditorGetSize:
		if (editor) {
			return editor->getRect((ERect**)ptr) ? 1 : 0;
		}
		break;
	case audioEffectEditorOpen:
		if (editor) {
			return editor->open(ptr);
		}
		break;
	case audioEffectEditorClose:
		if (editor) {
			editor->close();
		}
		break;
	case audioEffectEditorDoNothing: // Occasionally appears. Does nothing? Only called when editor is open.
		if (editor) {
			editor->do_nothing();
		}
		break;
	case audioEffectGetChunk:
		return getChunk((void**)ptr, !!param1);
	case audioEffectSetChunk:
		return setChunk(ptr, static_cast<VstInt32>(value), !!param1);
	default:
		break;
	}
	return 0;
}

AEffect* AudioEffect::getAeffect()
{
	return &cEffect;
}

void AudioEffect::setEditor(AEffEditor* p_editor)
{
	editor = p_editor;
}

void AudioEffect::setUniqueID(VstInt32 uniqueID)
{
	cEffect.uniqueID = uniqueID;
}

void AudioEffect::setNumInputs(VstInt32 numInputs)
{
	cEffect.numInputs = numInputs;
}

void AudioEffect::setNumOutputs(VstInt32 numOutputs)
{
	cEffect.numOutputs = numOutputs;
}

void AudioEffect::setInitialDelay(VstInt32 delay)
{
	cEffect.delay = delay;
}

void AudioEffect::canProcessReplacing(bool value)
{
	if (value)
		cEffect.flags |= AEffectFlagCanReplacing;
	else
		cEffect.flags &= ~AEffectFlagCanReplacing;
}

void AudioEffect::isSynth(bool value)
{
	if (value)
		cEffect.flags |= AEffectFlagIsSynthesizer;
	else
		cEffect.flags &= ~AEffectFlagIsSynthesizer;
}

void AudioEffect::programsAreChunks(bool value)
{
	if (value)
		cEffect.flags |= AEffectFlagProgramsAreChunks;
	else
		cEffect.flags &= ~AEffectFlagProgramsAreChunks;
}

void AudioEffect::setParameterAutomated(VstInt32 index, float value)
{
	setParameter(index, value);
	if (audioMaster)
		audioMaster(getAeffect(), audioMasterAutomate, index, 0, 0, value);
}
