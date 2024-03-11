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
#include "vstkey.h"
#include "vsttypes.h"

#pragma pack(push, 8)

class AudioEffect;

class AEffEditor {
	public:
	AEffEditor(AudioEffect* effect);
	virtual ~AEffEditor();

	virtual bool getRect(ERect** rect)
	{
		*rect = nullptr;
		return false;
	}
	virtual bool open(void* ptr);
	virtual void close();
	virtual void do_nothing()
	{
		return;
	}

#if (!defined VST_VERSION_SUPPORT) || (VST_VERSION_SUPPORT >= kVstVersion_2100)
	virtual bool setKnobMode(VstInt32 val)
	{
		return false;
	}
	virtual bool onKeyDown(VstKeyCode& keyCode)
	{
		return false;
	}
	virtual bool onKeyUp(VstKeyCode& keyCode)
	{
		return false;
	}
	virtual bool onWheel(float distance)
	{
		return false;
	}
#endif

	protected:
	AudioEffect* effect; // Restricted to this name by vst2wrapper.
	void*        _handle;
};

#pragma pack(pop)
