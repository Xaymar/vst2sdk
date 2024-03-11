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
#include "aeffectx.h"
#include "audioeffect.hpp"
#include "vstmidi.h"
#include "vstpin.h"
#include "vstspeaker.h"
#include "vsttypes.h"

class AudioEffectX : public AudioEffect {
	public:
	AudioEffectX(audioMasterCallback audioMaster, VstInt32 __unk00, VstInt32 __unk01);
	virtual ~AudioEffectX();

	protected:
	// Internal
	virtual VstIntPtr control(VstInt32 opcode, VstInt32 param1, VstIntPtr param2, void* ptr, float value) override;

	public:
	// AEffect/AEffectX
	virtual void canDoubleReplacing(bool value);
	virtual void noTail(bool value);

	virtual void processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames){};

	// Audio Master (sorted by AEffectMasterOpCodes)
	virtual VstTimeInfo* getTimeInfo(VstInt32 filter);
	virtual bool         sendVstEventsToHost(VstEvents* events);
	virtual bool         ioChanged();
	virtual bool         sizeWindow(VstInt32 width, VstInt32 height);
	virtual VstInt32     getCurrentProcessLevel();
	virtual bool         getHostVendorString(char* text);
	virtual bool         getHostProductString(char* text);
	virtual VstInt32     getHostVendorVersion();
	virtual VstInt32     canHostDo(char* text);
	virtual bool         beginEdit(VstInt32 paramIndex);
	virtual bool         endEdit(VstInt32 paramIndex);
	virtual bool         updateDisplay();

	// Audio Plugin (sorted by AEffectOpCodes)
	virtual VstInt32 processEvents(VstEvents* events)
	{
		return 0;
	}
	virtual bool canParameterBeAutomated(VstInt32 index)
	{
		return false;
	}
	virtual bool string2parameter(VstInt32 index, char* text)
	{
		return false;
	}
	virtual bool getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
	{
		return false;
	}
	virtual bool getInputProperties(VstInt32 index, VstPinProperties* properties)
	{
		return false;
	}
	virtual bool getOutputProperties(VstInt32 index, VstPinProperties* properties)
	{
		return false;
	}
	virtual VstPlugCategory getPlugCategory()
	{
		return kPlugCategUnknown;
	}
	virtual bool setSpeakerArrangement(VstSpeakerArrangement* pluginInput, VstSpeakerArrangement* pluginOutput)
	{
		return false;
	}
	virtual bool setBypass(bool onOff)
	{
		return false;
	}
	virtual bool getEffectName(char* name)
	{
		return false;
	}
	virtual bool getVendorString(char* text)
	{
		return false;
	}
	virtual bool getProductString(char* text)
	{
		return false;
	}
	virtual VstInt32 getVendorVersion()
	{
		return 0;
	}
	virtual VstIntPtr vendorSpecific(VstInt32 lArg, VstIntPtr lArg2, void* ptrArg, float floatArg)
	{
		return 0;
	}
	virtual VstInt32 canDo(char* text)
	{
		return 0;
	}
	virtual VstInt32 getGetTailSize()
	{
		return 0;
	}
	virtual bool getParameterProperties(VstInt32 index, VstParameterProperties* p)
	{
		return false;
	}
	virtual VstInt32 getMidiProgramName(VstInt32 channel, MidiProgramName* midiProgramName)
	{
		return 0;
	}
	virtual VstInt32 getCurrentMidiProgram(VstInt32 channel, MidiProgramName* currentProgram)
	{
		return 0;
	}
	virtual VstInt32 getMidiProgramCategory(VstInt32 channel, MidiProgramCategory* category)
	{
		return 0;
	}
	virtual bool hasMidiProgramsChanged(VstInt32 channel)
	{
		return false;
	}
	virtual bool getMidiKeyName(VstInt32 channel, MidiKeyName* keyName)
	{
		return false;
	}
	virtual bool getSpeakerArrangement(VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput)
	{
		return false;
	}
	virtual VstInt32 startProcess()
	{
		return 0;
	}
	virtual VstInt32 stopProcess()
	{
		return 0;
	}
	virtual bool setProcessPrecision(VstInt32 precision)
	{
		return false;
	}
	virtual VstInt32 getNumMidiInputChannels()
	{
		return 0;
	}
	virtual VstInt32 getNumMidiOutputChannels()
	{
		return 0;
	}
};
