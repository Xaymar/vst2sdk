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
#include "aeffect.h"
#include "aeffeditor.h"
#include "vstevent.h"
#include "vstkey.h"
#include "vstparameter.h"
#include "vstpin.h"
#include "vstspeaker.h"
#include "vsttypes.h"

// This may work. Or it may not. We don't guarantee compatibility with the original.
class AudioEffect {
	public:
	AudioEffect(audioMasterCallback audioMaster, VstInt32 __unk00, VstInt32 __unk01);
	virtual ~AudioEffect();

	protected:
	// Internal
	virtual VstIntPtr control(VstInt32 opcode, VstInt32 param1, VstIntPtr param2, void* ptr, float value);

	public:
	// AudioEffect
	virtual AEffect* getAeffect();
	virtual void     setEditor(AEffEditor* editor);

	virtual void process(float** inputs, float** outputs, VstInt32 sampleFrames){};

	virtual float getParameter(VstInt32 index)
	{
		return 0;
	}
	virtual void setParameter(VstInt32 index, float value){};

	virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames){};

	// AEffect
	virtual void setUniqueID(VstInt32 uniqueID);
	virtual void setNumInputs(VstInt32 numInputs);
	virtual void setNumOutputs(VstInt32 numOutputs);
	virtual void setInitialDelay(VstInt32 delay);
	virtual void canProcessReplacing(bool value);
	virtual void isSynth(bool value);
	virtual void programsAreChunks(bool value);

	// Audio Master (sorted by AEffectMasterOpCodes)
	virtual void setParameterAutomated(VstInt32 index, float value);

	// Audio Plugin (sorted by AEffectOpCodes)
	virtual void     open(){};
	virtual void     close(){};
	virtual void     setProgram(VstInt32 program){};
	virtual VstInt32 getProgram()
	{
		return 0;
	}
	virtual void setProgramName(char* name){};
	virtual void getProgramName(char* name)
	{
		*name = 0;
	}
	virtual void     getParameterLabel(VstInt32 index, char* label){};
	virtual void     getParameterDisplay(VstInt32 index, char* text){};
	virtual void     getParameterName(VstInt32 index, char* text){};
	virtual void     setSampleRate(float newSamplerate){};
	virtual void     setBlockSize(VstInt32 newBlockSize){};
	virtual void     suspend(){};
	virtual void     resume(){};
	virtual VstInt32 getChunk(void** data, bool isPreset = false)
	{
		return 0;
	}
	virtual VstInt32 setChunk(void* data, VstInt32 byteSize, bool isPreset = false)
	{
		return 0;
	}

	// Defined by Implementation

	protected: // Needs to be accessible by AudioEffectX
	AEffect             cEffect; // vst2wrapper requires this.
	audioMasterCallback audioMaster; // vst2wrapper requires this.
	VstInt32            numPrograms;
	VstInt32            curProgram;
	AEffEditor*         editor;
};
