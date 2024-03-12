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

#include "audioeffectx.hpp"
#include "vstkey.h"

AudioEffectX::AudioEffectX(audioMasterCallback audioMaster, VstInt32 __unk00, VstInt32 __unk01) : AudioEffect(audioMaster, __unk00, __unk01)
{
	cEffect.processDoubleReplacing = [](AEffect* self, double** inputs, double** outputs, VstInt32 sampleFrames) { return reinterpret_cast<AudioEffectX*>(self->effect_data)->processDoubleReplacing(inputs, outputs, sampleFrames); };
}

AudioEffectX::~AudioEffectX() {}

VstIntPtr AudioEffectX::control(VstInt32 opcode, VstInt32 param1, VstIntPtr param2, void* ptr, float value)
{
	switch (opcode) {
	case audioEffectProcessEvents:
		return processEvents((VstEvents*)ptr);
	case audioEffectIsParameterAutomatable:
		return canParameterBeAutomated(param1) ? 1 : 0;
	case audioEffectConvertStringToParameter:
		return string2parameter(param1, (char*)ptr) ? 1 : 0;
	case audioEffectGetProgramNameIndexed:
		return getProgramNameIndexed(static_cast<VstInt32>(value), param1, (char*)ptr) ? 1 : 0;
	case audioEffectGetInputProperties:
		return getInputProperties(param1, (VstPinProperties*)ptr);
	case audioEffectGetOutputProperties:
		return getOutputProperties(param1, (VstPinProperties*)ptr);
	case audioEffectGetPlugCategory:
		return (VstIntPtr)getPlugCategory();
	case audioEffectSetSpeakerArrangement:
		return setSpeakerArrangement((VstSpeakerArrangement*)param2, (VstSpeakerArrangement*)ptr);
	case audioEffectBypass:
		return setBypass(!!param2) ? 1 : 0;
	case audioEffectGetEffectName:
		return getEffectName((char*)ptr) ? 1 : 0;
	case audioEffectGetVendorString:
		return getVendorString((char*)ptr) ? 1 : 0;
	case audioEffectGetProductString:
		return getProductString((char*)ptr) ? 1 : 0;
	case audioEffectGetVendorVersion:
		return getVendorVersion();
	case audioEffectVendorSpecific:
		return vendorSpecific(param1, param2, ptr, value);
	case audioEffectCanDo:
		return canDo((char*)ptr);
	case audioEffectGetTailSize:
		return getGetTailSize();
	case audioEffectGetParameterProperties:
		return getParameterProperties(param1, (VstParameterProperties*)ptr) ? 1 : 0;
	case audioEffectGetVSTVersion:
		return cEffect.version;

#if (!defined VST_VERSION_SUPPORT) || (VST_VERSION_SUPPORT >= kVstVersion_2100)
	case audioEffectEditorKeyDown:
		if (editor) {
			VstKeyCode keycode{.character = param1, .virt = static_cast<unsigned char>(param2), .modifier = static_cast<unsigned char>(value)};
			editor->onKeyDown(keycode);
		}
		return 0;
	case audioEffectEditorKeyUp:
		if (editor) {
			VstKeyCode keycode{.character = param1, .virt = static_cast<unsigned char>(param2), .modifier = static_cast<unsigned char>(value)};
			editor->onKeyUp(keycode);
		}
		return 0;
	case audioEffectEditorKnobMode:
		if (editor) {
			editor->setKnobMode(param2) ? 1 : 0;
		}
		return 0;
	case audioEffectMidiGetProgramName:
		return getMidiProgramName(param1, (MidiProgramName*)ptr);
	case audioEffectMidiGetCurrentProgram:
		return getCurrentMidiProgram(param1, (MidiProgramName*)ptr);
	case audioEffectMidiGetProgramCategory:
		return getMidiProgramCategory(param1, (MidiProgramCategory*)ptr);
	case audioEffectMidiHasProgramsChanged:
		return hasMidiProgramsChanged(param1) ? 1 : 0;
	case audioEffectMidiGetKeyName:
		return getMidiKeyName(param1, (MidiKeyName*)ptr) ? 1 : 0;
#endif

#if (!defined VST_VERSION_SUPPORT) || (VST_VERSION_SUPPORT >= kVstVersion_2300)
	case audioEffectGetSpeakerArrangement:
		return getSpeakerArrangement((VstSpeakerArrangement**)param2, (VstSpeakerArrangement**)ptr);
	case audioEffectStartProcessing:
		return startProcess();
	case audioEffectStopProcessing:
		return stopProcess();
#endif

#if (!defined VST_VERSION_SUPPORT) || (VST_VERSION_SUPPORT >= kVstVersion_2400)
	case audioEffectSetProcessingPrecision:
		return setProcessPrecision(param1) ? 1 : 0;
	case audioEffectMidiGetNumInputChannels:
		return getNumMidiInputChannels();
	case audioEffectMidiGetNumOutputChannels:
		return getNumMidiOutputChannels();
#endif
	default:
		return AudioEffect::control(opcode, param1, param2, ptr, value);
	}
}

VstInt32 AudioEffectX::canHostDo(char* text)
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterCanDo, 0, 0, text, 0);
	return false;
}

bool AudioEffectX::beginEdit(VstInt32 paramIndex)
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterBeginEdit, paramIndex, 0, 0, 0);
	return false;
}

bool AudioEffectX::endEdit(VstInt32 paramIndex)
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterEndEdit, paramIndex, 0, 0, 0);
	return false;
}

bool AudioEffectX::ioChanged()
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterIOChanged, 0, 0, 0, 0);
	return false;
}

bool AudioEffectX::updateDisplay()
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterUpdateDisplay, 0, 0, 0, 0);
	return false;
}

bool AudioEffectX::sizeWindow(VstInt32 width, VstInt32 height)
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterSizeWindow, width, height, 0, 0);
	return false;
}

VstInt32 AudioEffectX::getCurrentProcessLevel()
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterGetCurrentProcessLevel, 0, 0, 0, 0);
	return 0;
}

bool AudioEffectX::getHostVendorString(char* text)
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterGetVendorString, 0, 0, text, 0);
	return false;
}

bool AudioEffectX::getHostProductString(char* text)
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterGetProductString, 0, 0, text, 0);
	return false;
}

VstInt32 AudioEffectX::getHostVendorVersion()
{
	if (audioMaster)
		return !!audioMaster(getAeffect(), audioMasterGetVendorVersion, 0, 0, 0, 0);
	return 0;
}

void AudioEffectX::canDoubleReplacing(bool value)
{
	if (value)
		cEffect.flags |= AEffectFlagCanDoubleReplacing;
	else
		cEffect.flags &= ~AEffectFlagCanDoubleReplacing;
}

void AudioEffectX::noTail(bool value)
{
	if (value)
		cEffect.flags |= AEffectFlagIsSilentOnSilence;
	else
		cEffect.flags &= ~AEffectFlagIsSilentOnSilence;
}

VstTimeInfo* AudioEffectX::getTimeInfo(VstInt32 filter)
{
	if (audioMaster) {
		auto ptr = audioMaster(getAeffect(), audioMasterGetTimeInfo, 0, filter, 0, 0);
		// Returned value should be a pointer to a VstTimeInfo.
		return reinterpret_cast<VstTimeInfo*>(ptr);
	}
	return 0;
}

bool AudioEffectX::sendVstEventsToHost(VstEvents* events)
{
	if (audioMaster) {
		audioMaster(getAeffect(), audioMasterProcessEvents, 0, 0, events, 0);
	}
	return 0;
}
