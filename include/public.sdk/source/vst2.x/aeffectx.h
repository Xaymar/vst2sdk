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

// File name based on "AudioEffectX" and "AudioEffect". Since we have a struct called "AEffect", logically there's a struct AEffectX right? Don't see any references to it though...

#pragma once
#include "aeffect.h"
#include "vstpin.h"
#include "vsttime.h"
#include "vstversion.h"

enum AEffectXFlags {
	AEffectFlagCanDoubleReplacing = 1 << 12, // Only available in 2.4 or later. Name based on AudioEffect::canDoubleReplacing
};

enum AEffectXOpCodes {
	__AEffectXOpCode_Lowest = 0x18, // Starts at highest OpCode from AEffect.
	AEffectXOpCode_19,
	audioEffectProcessEvents, // Process Events
	audioEffectIsParameterAutomatable, // Is Parameter Automatable?
	audioEffectConvertStringToParameter, // Convert String to Parameter index
	audioEffectGetProgramNameIndexed,
	AEffectXOpCode_1E,
	AEffectXOpCode_1F,
	AEffectXOpCode_20,
	audioEffectGetInputProperties, // Get Input Properties
	audioEffectGetOutputProperties, // Get Output Properties
	audioEffectGetPlugCategory, // Get "Plug" Category
	AEffectXOpCode_24,
	AEffectXOpCode_25,
	AEffectXOpCode_26,
	AEffectXOpCode_27,
	AEffectXOpCode_28,
	AEffectXOpCode_29,
	audioEffectSetSpeakerArrangement, // Set Speaker Arrangment
	AEffectXOpCode_2B,
	audioEffectBypass, // Bypass
	audioEffectGetEffectName, // Get Effect Name
	AEffectXOpCode_2E,
	audioEffectGetVendorString, // Get Vendor Name
	audioEffectGetProductString, // Get Product Name
	audioEffectGetVendorVersion, // Get Vendor Version
	audioEffectVendorSpecific, // Seems to be used rarely to implement host-specific custom behavior.
	audioEffectCanDo, // Test for Feature support
	audioEffectGetTailSize, // Get Tail Samples
	AEffectXOpCode_35,
	AEffectXOpCode_36,
	AEffectXOpCode_37,
	audioEffectGetParameterProperties,
	AEffectXOpCode_39,
	audioEffectGetVSTVersion, // Get VST Version

#if (!defined VST_VERSION_SUPPORT) || (VST_VERSION_SUPPORT >= kVstVersion_2100)
	// Only seen in version 2.1 or later
	audioEffectEditorKeyDown, // Key Down in Editor.
	audioEffectEditorKeyUp, // Key Up in Editor
	audioEffectEditorKnobMode, // Editor Knob Mode, see CKnobMode in vstgui.
	audioEffectMidiGetProgramName,
	audioEffectMidiGetCurrentProgram,
	audioEffectMidiGetProgramCategory,
	audioEffectMidiHasProgramsChanged,
	audioEffectMidiGetKeyName,
	AEffectXOpCode_43,
	AEffectXOpCode_44,
#endif

#if (!defined VST_VERSION_SUPPORT) || (VST_VERSION_SUPPORT >= kVstVersion_2300)
	// Only seen in version 2.3 or later
	audioEffectGetSpeakerArrangement, // Get Speaker Arrangement
	AEffectXOpCode_46,
	audioEffectStartProcessing, // Start processing audio.
	audioEffectStopProcessing, // Stop processing audio.
	AEffectXOpCode_49,
	AEffectXOpCode_4A,
	AEffectXOpCode_4B,
	AEffectXOpCode_4C,
#endif

#if (!defined VST_VERSION_SUPPORT) || (VST_VERSION_SUPPORT >= kVstVersion_2400)
	// Only seen in version 2.4 or later.
	audioEffectSetProcessingPrecision, // Set Processing Precision, see VstProcessPrecision
	audioEffectMidiGetNumInputChannels,
	audioEffectMidiGetNumOutputChannels,
#endif

	// Never seen any OpCodes at or above this maximum.
	AEffectXOpCodeMax
};

enum AEffectXMasterOpCodes {
	__AEffectXMasterOpCodeLowest = 0x04,
	AEffectXMasterOpCode_05, // Appears to be unused entirely.
	AEffectXMasterOpCode_06,
	audioMasterGetTimeInfo, // Get Time Info. Name guessed based on AudioEffectX::getTimeInfo
	audioMasterProcessEvents, // Name guessed based on AudioEffectX::sendVstEventsToHost. Name is likely a mirror of audioEffectsProcessEvents.
	AEffectXMasterOpCode_09,
	AEffectXMasterOpCode_0A,
	AEffectXMasterOpCode_0B,
	AEffectXMasterOpCode_0C,
	audioMasterIOChanged, // Notify Host that our Inputs/Outputs/etc have changed. Name guessed based on AudioEffectX::ioChanged
	AEffectXMasterOpCode_0E,
	audioMasterSizeWindow, // Request a resize of the editor window. Name guessed based on AudioEffectX::sizeWindow
	AEffectXMasterOpCode_10,
	AEffectXMasterOpCode_11,
	AEffectXMasterOpCode_12,
	AEffectXMasterOpCode_13,
	AEffectXMasterOpCode_14,
	AEffectXMasterOpCode_15,
	AEffectXMasterOpCode_16,
	audioMasterGetCurrentProcessLevel, // Name guessed based on AudioEffectX::getCurrentProcessLevel
	AEffectXMasterOpCode_18,
	AEffectXMasterOpCode_19,
	AEffectXMasterOpCode_1A,
	AEffectXMasterOpCode_1B,
	AEffectXMasterOpCode_1C,
	AEffectXMasterOpCode_1D,
	AEffectXMasterOpCode_1E,
	AEffectXMasterOpCode_1F,
	audioMasterGetVendorString, // Get Vendor Name
	audioMasterGetProductString, // Get Product Name
	audioMasterGetVendorVersion, // Get Vendor Version
	audioMasterVendorSpecific, // Counterpart to AEffectXOpCode_32
	audioMasterCanDo,
	AEffectXMasterOpCode_25,
	AEffectXMasterOpCode_26,
	AEffectXMasterOpCode_27,
	AEffectXMasterOpCode_28,
	AEffectXMasterOpCode_29, // When called, returns a pointer to a string.
	audioMasterUpdateDisplay, // Request an update for the editor display? Name guessed via AudioEffectX::updateDisplay
	audioMasterBeginEdit, // Parameter Gained Focus. Likely used to help the host ignore unwanted inputs. Name guessed via AudioEffectX::beginEdit
	audioMasterEndEdit, // Parameter Lost Focus. Likely used to help the host ignore unwanted inputs. Name guessed via AudioEffectX::endEdit
	AEffectXMasterOpCode_2D,
	AEffectXMasterOpCode_2E,
	AEffectXMasterOpCode_2F,
	AEffectXMasterOpCode_30,
	AEffectXMasterOpCode_31,

	AEffectXMasterOpCodeMax
};

enum VstPlugCategory { // Based on OpCode being here only.
	// Taken from VstPlugCategory Vst2Wrapper::getPlugCategory (), then shuffled around until it made sense in most VST hosts.
	kPlugCategUnknown = 0,
	kPlugCategEffect,
	kPlugCategSynth,
	kPlugCategAnalysis,
	kPlugCategMastering,
	kPlugCategSpacializer,
	kPlugCategRoomFx,
	kPlugCateg__unk00, // Unknown use.
	kPlugCategRestoration,
	kPlugCateg__unk01, // Unknown use.
	kPlugCategContainer, // AEffectX contains multiple plugins. Not clear how this works.
	kPlugCategGenerator,
};

enum VstProcessLevels {
	// No idea what the rest are.
	kVstProcessLevelRealtime = 2,
	kVstProcessLevelOffline  = 4,
};

enum VstProcessPrecision {
	kVstProcessPrecision32, // Single
	kVstProcessPrecision64, // Double
};
