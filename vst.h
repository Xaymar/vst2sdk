/* An attempt at an untained clean room reimplementation of the widely popular VST 2.x SDK.
 * Copyright (c) 2020 Xaymar Dirks <info@xaymar.com> (previously known as Michael Fabian Dirks)
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *    disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Please refer to README.md and LICENSE for further information.

// Protect against double inclusion in practically every compiler available.
#pragma once
#ifndef VST2SDK_VST_H
#define VST2SDK_VST_H

#define VST_FUNCTION_INTERFACE __cdecl // Incorrect for older Windows platforms. Is it actually stdcall sometimes? That would be stupid.
#define VST_ALIGNMENT 8 // This appears to be wrong for 32-bit. But it's not 4 byte either? What.
#define VST_MAGICNUMBER 'VstP'
#define VST_MAX_CHANNELS 32 // Couldn't find any audio editing software which would attempt to add more channels.

#pragma pack(push, VST_ALIGNMENT)

#ifdef __cplusplus
#include <cinttypes>
extern "C" {
#else
#include <inttypes.h>
#endif

/** Known Status Codes
 */
enum VST_STATUS {
	VST_STATUS_0 = 0,
	VST_STATUS_FALSE = 0,
	VST_STATUS_ERROR = 0,
	VST_STATUS_UNKNOWN = 0,

	VST_STATUS_1 = 1,
	VST_STATUS_TRUE = 1,
	VST_STATUS_SUCCESS = 1,
	VST_STATUS_YES = 1,

	// Only seen in one single op-code.
	VST_STATUS_m1 = -1,
	VST_STATUS_NOT_IMPLEMENTED = -1, // Not an official status, but seen in some Reaper-style VSTs.
	VST_STATUS_NO = -1,

	_VST_STATUS_PAD = 0xFFFFFFFFul,
};

/** Known Buffer Sizes
 */
enum VST_BUFFER_SIZE {
	VST_BUFFER_SIZE_PARAM_LABEL = 8,
	VST_BUFFER_SIZE_PARAM_LONG_NAME = 64,
	VST_BUFFER_SIZE_PARAM_NAME = 8,
	VST_BUFFER_SIZE_PARAM_VALUE = 8,
	VST_BUFFER_SIZE_PROGRAM_NAME = 24,
	VST_BUFFER_SIZE_CATEGORY_LABEL = 24,
	VST_BUFFER_SIZE_EFFECT_NAME = 32,
	VST_BUFFER_SIZE_VENDOR_NAME = 64,
	VST_BUFFER_SIZE_PRODUCT_NAME = 64,
	VST_BUFFER_SIZE_SPEAKER_NAME = 64,
}; // This is an enum because I started to dislike macros.

/** VST Version Enumeration
 * - The VST version format is in Base10.
 * - It is very weird, either as a single number (A -> A.0.0.0), or as a group of 4 numbers (ABCD -> A.B.C.D).
 */
enum VST_VERSION {
	VST_VERSION_1       = 0, // Anything before 2.0, used by official plug-ins.
	VST_VERSION_1_0_0_0 = 1000, // 1.0, used by some third-party plug-ins.
	VST_VERSION_1_1_0_0 = 1100, // 1.1, used by some third-party plug-ins.
	VST_VERSION_2       = 2, // 2.0, used by official plug-ins.
	VST_VERSION_2_0_0_0 = 2000, // 2.0, used by some third-party plug-ins.
	VST_VERSION_2_1_0_0 = 2100, // 2.1
	VST_VERSION_2_2_0_0 = 2200, // 2.2
	VST_VERSION_2_3_0_0 = 2300, // 2.3
	VST_VERSION_2_4_0_0 = 2400, // 2.4

	// Pad to force 32-bit number.
	_VST_VERSION_PAD = 0xFFFFFFFFul,
};

/** Window/Editor Rectangle
 */
struct vst_rect {
	int16_t top;
	int16_t left;
	int16_t bottom;
	int16_t right;
};

enum VST_PARAMETER_FLAG {
	/** Parameter is an on/off switch.
	 *
	 */
	VST_PARAMETER_FLAG_SWITCH = 1,

	/** Parameter limits are set as integers.
	 *
	 */
	VST_PARAMETER_FLAG_INTEGER_LIMITS = 1 << 1,

	/** Parameter uses float steps.
	 *
	 */
	VST_PARAMETER_FLAG_STEP_FLOAT = 1 << 2,

	/** Parameter uses integer steps.
	 *
	 */
	VST_PARAMETER_FLAG_STEP_INT = 1 << 3,

	/** Parameter has an display order index for the default editor.
	 *
	 * Only applies to the default editor.
	 */
	VST_PARAMETER_FLAG_INDEX = 1 << 4,

	/** Parameter has a category for the default editor.
	 *
	 * Only applies to the default editor.
	 */
	VST_PARAMETER_FLAG_CATEGORY = 1 << 5,

	/** Parameter can be gradually increased/decreased.
	 * 
	 */
	VST_PARAMETER_FLAG_RAMPING = 1 << 6,

	_VST_PARAMETER_FLAG_PAD     = 0xFFFFFFFFul,
};

struct vst_parameter_properties_t {
	float step_f32;
	float step_small_f32;
	float step_large_f32;

	/** Human-readable name for this parameter.
	 * 
	 * Note: Only applies to the default editor.
	 */
	char name[VST_BUFFER_SIZE_PARAM_LONG_NAME];

	/** Parameter Flags
	 * 
	 * Any combination of VST_PARAMETER_FLAGS.
	 */
	uint32_t flags;

	int32_t  min_value_i32;
	int32_t  max_value_i32;
	int32_t  step_i32;

	/** Short Human-readable label for this parameter.
	 * 
	 * I have no idea why this exists?
	 * Note: Only applies to the default editor.
	 */
	char label[VST_BUFFER_SIZE_PARAM_LABEL];

	/**
	 * 
	 * Note: Only applies to the default editor.
	 * Note: Valid if VST_PARAMETER_FLAG_INDEX is set.
	 */
	uint16_t index;

	/** Category index
	 * 
	 * Must either be 0 for no category, or any number increasing from 1 onwards.
	 * 
	 * Note: Only applies to the default editor.
	 * Note: Valid if VST_PARAMETER_FLAG_CATEGORY is set.
	 */
	uint16_t category;

	/** How many parameters are in this category?
	 * 
	 * Question: Can't the host just figure this out by itself?!
	 * Note: Only applies to the default editor.
	 * Note: Valid if VST_PARAMETER_FLAG_CATEGORY is set.
	 */
	uint16_t num_parameters_in_category;

	uint16_t _unknown_00 = 0; // Must be set to 0.

	/** Human-readable name for the category this parameter is in.
	 * 
	 * Note: Only applies to the default editor.
	 * Note: Valid if VST_PARAMETER_FLAG_CATEGORY is set.
	 */
	char category_label[VST_BUFFER_SIZE_CATEGORY_LABEL];

	char _reserved[16]; // Reserved for future expansions?
};

enum VST_SPEAKER_TYPE {
	// Default Types
	VST_SPEAKER_TYPE_MONO       = 0,
	VST_SPEAKER_TYPE_LEFT       = 1,
	VST_SPEAKER_TYPE_RIGHT      = 2,
	VST_SPEAKER_TYPE_CENTER     = 3,
	VST_SPEAKER_TYPE_LFE        = 4,
	VST_SPEAKER_TYPE_LEFT_REAR  = 5, // Rear/Surround Left
	VST_SPEAKER_TYPE_RIGHT_REAR = 6, // Rear/Surround Right
	// 7
	// 8
	// 9
	VST_SPEAKER_TYPE_LEFT_SIDE  = 10, // Side Left
	VST_SPEAKER_TYPE_RIGHT_SIDE = 11, // Side Right
	// 12
	// 13
	// 14
	// 15
	// ...

	// User Types (seen rarely, but never exceeds -32)
	VST_SPEAKER_TYPE_USER_32 = -32,
	VST_SPEAKER_TYPE_USER_31,
	VST_SPEAKER_TYPE_USER_30,
	VST_SPEAKER_TYPE_USER_29,
	VST_SPEAKER_TYPE_USER_28,
	VST_SPEAKER_TYPE_USER_27,
	VST_SPEAKER_TYPE_USER_26,
	VST_SPEAKER_TYPE_USER_25,
	VST_SPEAKER_TYPE_USER_24,
	VST_SPEAKER_TYPE_USER_23,
	VST_SPEAKER_TYPE_USER_22,
	VST_SPEAKER_TYPE_USER_21,
	VST_SPEAKER_TYPE_USER_20,
	VST_SPEAKER_TYPE_USER_19,
	VST_SPEAKER_TYPE_USER_18,
	VST_SPEAKER_TYPE_USER_17,
	VST_SPEAKER_TYPE_USER_16,
	VST_SPEAKER_TYPE_USER_15,
	VST_SPEAKER_TYPE_USER_14,
	VST_SPEAKER_TYPE_USER_13,
	VST_SPEAKER_TYPE_USER_12,
	VST_SPEAKER_TYPE_USER_11,
	VST_SPEAKER_TYPE_USER_10,
	VST_SPEAKER_TYPE_USER_09,
	VST_SPEAKER_TYPE_USER_08,
	VST_SPEAKER_TYPE_USER_07,
	VST_SPEAKER_TYPE_USER_06,
	VST_SPEAKER_TYPE_USER_05,
	VST_SPEAKER_TYPE_USER_04,
	VST_SPEAKER_TYPE_USER_03,
	VST_SPEAKER_TYPE_USER_02,
	VST_SPEAKER_TYPE_USER_01,


	// Pad to force 32-bit number.
	_VST_SPEAKER_TYPE_PAD = 0xFFFFFFFFul,
};

struct vst_speaker_properties_t {
	/** Azimuth in Radians
	 * Range: -PI to PI
	 * 
	 * Note: Must be 10.0 if this is a LFE.
	 */
	float azimuth;

	/** Altitude in Radians
	 * Range: -PI/2 to PI/2
	 * 
	 * Note: Must be 10.0 if this is a LFE.
	 */
	float altitude;

	/** Distance in Meters
	 * range: 0 to +-Infinity
	 * 
	 * Note: Must be 0.0 if this is a LFE.
	 */
	float distance;

	float _unknown_00 = 0;

	/** Human readable name for this speaker.
	 * 
	 * Some hosts will behave weird if you use "L", "R", "C", "Ls", "Rs", "Lc", "Rc", "LFE", "Lfe", "Sl", "Sr", "Cs", 
	 * and other 2 to 3 letter short codes. Best not to use those if you like your plug-in in a not-crashy state.
	 */
	char name[VST_BUFFER_SIZE_SPEAKER_NAME];

	/** The type of the speaker
	 * 
	 * See VST_SPEAKER_TYPE
	 * 
	 * If the above is one of those short codes some host seems to overwrite this with their own. Memory safety is 
	 * optional apparently.
	 */
	int32_t type;

	uint8_t _reserved[28]; // Reserved for future expansions?
};

enum VST_SPEAKER_ARRANGEMENT_TYPE {
	/** Custom speaker arrangement.
	 *
	 * Accidentally discovered through random testing.
	 */
	VST_SPEAKER_ARRANGEMENT_TYPE_CUSTOM = -2,

	/** Unknown/Empty speaker layout.
	 */
	VST_SPEAKER_ARRANGEMENT_TYPE_UNKNOWN = -1,

	/** Mono
	 */
	VST_SPEAKER_ARRANGEMENT_TYPE_MONO = 0x00,

	/** Stereo
	 */
	VST_SPEAKER_ARRANGEMENT_TYPE_STEREO = 0x01,
	
	/** Quadraphonic
	 */
	VST_SPEAKER_ARRANGEMENT_TYPE_4_0 = 0x0B,

	/** 5.0 (Old Surround)
	 * 
	 * L, R, C, RL, RR
	 */
	VST_SPEAKER_ARRANGEMENT_TYPE_5_0 = 0x0E,

	/** 5.1 (Old Surround)
	 * 
	 * L, R, C, LFE, RL, RR
	 */
	VST_SPEAKER_ARRANGEMENT_TYPE_5_1 = 0x0F,

	/** 7.1 (Full Surround)
	 * 
	 * L, R, C, LFE, SL, SR, RL, RR
	 */
	VST_SPEAKER_ARRANGEMENT_TYPE_7_1 = 0x17,

	// Pad to force 32-bit number.
	_VST_SPEAKER_ARRANGEMENT_TYPE_PAD = 0xFFFFFFFFul,
};

struct vst_speaker_arrangement_t {
	int32_t type; // See VST_SPEAKER_ARRANGEMENT_TYPE
	int32_t channels; // Number of channels in this arrangement.
	vst_speaker_properties_t speakers[VST_MAX_CHANNELS]; // Array of speaker properties, actual size defined by channels.
};

//------------------------------------------------------------------------------------------------------------------------
// VST Host related Things
//------------------------------------------------------------------------------------------------------------------------

/** Plug-in to Host Op-Codes
 * These Op-Codes are emitted by the plug-in and the host _may_ handle them or return 0 (false).
 * We have no guarantees about anything actually happening.
 */
enum VST_HOST_OPCODE {
	/** Update automation for a given Parameter
	 * 
	 * Must be used to notify the host that the parameter was changed by the user if a custom editor is used.
	 * 
	 * @param p_int1 Parameter Index
	 * @param p_float Parameter Value
	 * @return Expected to return... something.
	 */
	VST_HOST_OPCODE_00 = 0x00, // cb(vst, 0x00, ?, 0, 0);
	VST_HOST_OPCODE_AUTOMATE = 0x00,
	
	/** Retrieve the Hosts VST Version.
	 * 
	 * @return See VST_VERSION enumeration.
	 */
	VST_HOST_OPCODE_01 = 0x01,
	VST_HOST_OPCODE_VST_VERSION = 0x01,

	VST_HOST_OPCODE_02 = 0x02, // bool cb(0, 0x02, 0, 0, 0);

	/** Some sort of idle keep-alive?
	 * 
	 * Seems to be called only in editor windows when a modal popup is present.
	 */
	VST_HOST_OPCODE_03 = 0x03,
	VST_HOST_OPCODE_KEEPALIVE_OR_IDLE = 0x03,

	VST_HOST_OPCODE_04 = 0x04,

	//--------------------------------------------------------------------------------
	// VST 2.x starts here.
	//--------------------------------------------------------------------------------

	VST_HOST_OPCODE_05 = 0x05,

	VST_HOST_OPCODE_06 = 0x06,	

	VST_HOST_OPCODE_07 = 0x07,

	VST_HOST_OPCODE_08 = 0x08,

	VST_HOST_OPCODE_09 = 0x09,

	VST_HOST_OPCODE_0A = 0x0A,

	VST_HOST_OPCODE_0B = 0x0B,

	VST_HOST_OPCODE_0C = 0x0C,

	VST_HOST_OPCODE_0D = 0x0D,

	VST_HOST_OPCODE_0E = 0x0E,

	VST_HOST_OPCODE_0F = 0x0F,

	VST_HOST_OPCODE_10 = 0x10,

	VST_HOST_OPCODE_11 = 0x11,

	VST_HOST_OPCODE_12 = 0x12,

	VST_HOST_OPCODE_13 = 0x13,

	VST_HOST_OPCODE_14 = 0x14,

	VST_HOST_OPCODE_15 = 0x15,

	VST_HOST_OPCODE_16 = 0x16,

	VST_HOST_OPCODE_17 = 0x17,

	VST_HOST_OPCODE_18 = 0x18,

	VST_HOST_OPCODE_19 = 0x19,

	VST_HOST_OPCODE_1A = 0x1A,

	VST_HOST_OPCODE_1B = 0x1B,

	VST_HOST_OPCODE_1C = 0x1C,

	VST_HOST_OPCODE_1D = 0x1D,

	VST_HOST_OPCODE_1E = 0x1E,

	VST_HOST_OPCODE_1F = 0x1F,

	VST_HOST_OPCODE_20 = 0x20,

	VST_HOST_OPCODE_21 = 0x21,

	VST_HOST_OPCODE_22 = 0x22,

	VST_HOST_OPCODE_23 = 0x23,

	VST_HOST_OPCODE_24 = 0x24,

	VST_HOST_OPCODE_25 = 0x25,

	VST_HOST_OPCODE_26 = 0x26,

	VST_HOST_OPCODE_27 = 0x27,

	VST_HOST_OPCODE_28 = 0x28,

	VST_HOST_OPCODE_29 = 0x29,

	/** Request an update of the editor window.
	 * 
	 */
	VST_HOST_OPCODE_2A = 0x2A,
	VST_HOST_OPCODE_EDITOR_UPDATE = 0x2A,

	/** Notify host that a parameter is being edited.
	 *
	 * @param p_int1 Parameter index.
	 */
	VST_HOST_OPCODE_2B = 0x2B,
	VST_HOST_OPCODE_PARAM_START_EDIT = 0x2B,

	/** Notify host that parameter is no longer being edited.
	 * 
	 * @param p_int1 Parameter index.
	 */
	VST_HOST_OPCODE_2C = 0x2C,
	VST_HOST_OPCODE_PARAM_STOP_EDIT = 0x2C,

	VST_HOST_OPCODE_2D = 0x2D,
	VST_HOST_OPCODE_2E = 0x2E,
	VST_HOST_OPCODE_2F = 0x2F,

	// Highest number of known OPCODE.
	VST_HOST_OPCODE_MAX,

	// Pad to force 32-bit number.
	_VST_HOST_OPCODE_PAD = 0xFFFFFFFFul,
};

/** Plug-in to Host callback
 *
 * The plug-in may call this to attempt to change things on the host side. The host side is free to ignore all requests, annoyingly enough.
 * 
 * @param opcode See VST_HOST_OPCODE
 * @param p_str Zero terminated string or null on call.
 * @return ?
 */
typedef intptr_t (*vst_host_callback)(vst_effect* plugin, VST_HOST_OPCODE opcode, int32_t p_int1, int64_t p_int2, const char* p_str, float p_float);

//------------------------------------------------------------------------------------------------------------------------
// VST Plug-in/Effect related Things
//------------------------------------------------------------------------------------------------------------------------

/** Plug-in Categories
 * All plug-ins must be in one of these categories and their behavior differs depending on which category they are in. 
 * This adds on to the "flags" each plug-in can specify.
 */
enum VST_EFFECT_CATEGORY {
	VST_EFFECT_CATEGORY_UNCATEGORIZED = 0x00,
	VST_EFFECT_CATEGORY_01            = 0x01,
	VST_EFFECT_CATEGORY_EFFECT        = 0x01, // Generic Effects, Distortion, ...
	VST_EFFECT_CATEGORY_02            = 0x02,
	VST_EFFECT_CATEGORY_INSTRUMENT    = 0x02, // Instruments, Synthesizers, Samplers, ...
	VST_EFFECT_CATEGORY_03            = 0x03,
	VST_EFFECT_CATEGORY_METERING      = 0x03, // Loudness Meters, Volume Analysis, etc.
	VST_EFFECT_CATEGORY_04            = 0x04,
	VST_EFFECT_CATEGORY_MASTERING     = 0x04, // Compressors, Limiters, ...
	VST_EFFECT_CATEGORY_05            = 0x05,
	VST_EFFECT_CATEGORY_SPATIAL       = 0x05, // Stereo and Multi-Channel Panning
	VST_EFFECT_CATEGORY_06            = 0x06,
	VST_EFFECT_CATEGORY_DELAY_OR_ECHO = 0x06, // Echo, Reverb, Room Simulator, ...
	VST_EFFECT_CATEGORY_07            = 0x07, 
	VST_EFFECT_CATEGORY_08            = 0x08,
	VST_EFFECT_CATEGORY_RESTORATION   = 0x08, // Denoising and similar effects.
	VST_EFFECT_CATEGORY_09            = 0x09,
	VST_EFFECT_CATEGORY_OFFLINE       = 0x09, // Offline Processing VST? Seems to receive all audio data prior to playback.
	VST_EFFECT_CATEGORY_0A            = 0x0A, 
	VST_EFFECT_CATEGORY_CONTAINER     = 0x0A, // Plugin contains more than one Plugin.
	VST_EFFECT_CATEGORY_0B            = 0x0B,
	VST_EFFECT_CATEGORY_WAVEGENERATOR = 0x0B, // Only found one released VST plugin with this, and it creates a perfect 400 hz sine wave?
	VST_EFFECT_CATEGORY_MAX, // Not part of specification, marks maximum category.

	// Pad to force 32-bit number.
	_VST_EFFECT_CATEGORY_PAD = 0xFFFFFFFFul,
};

/** Effect Flags
 */
enum VST_EFFECT_FLAG {
	VST_EFFECT_FLAG_EDITOR = 1 << 0, // The plug-in provides a custom editor instead of the generic host interface.
	//1 << 1,
	//1 << 2, // Only seen when the plug-in responds to VST_EFFECT_OPCODE_09. Seems to be ignored by hosts entirely.
	//1 << 3, // Only seen when the plug-in behaves differently in mono mode. Seems to be ignored by hosts entirely.
	VST_EFFECT_FLAG_SUPPORTS_FLOAT = 1 << 4, // Plug-in supports process_float. Must be set if VST version is 2.4.
	VST_EFFECT_FLAG_CHUNKS = 1 << 5, // Plug-in uses unformatted chunk data. 
	//1 << 6,
	//1 << 7,
	//1 << 8,
	VST_EFFECT_FLAG_INSTRUMENT = 1 << 8, // Plug-in is an instrument/generator of some kind.
	//1 << 9,
	VST_EFFECT_FLAG_NO_TAIL = 1 << 9, // Plug-in does not produce a tail. This is not the same as returning 1 in the tail samples op-code for some reason.
	//1 << 10,
	//1 << 11,
	//1 << 12,
	VST_EFFECT_FLAG_SUPPORTS_DOUBLE = 1 << 12, // Plug-in supports process_double. Optional mode for VST version 2.4, host can freely select.
};

/** Host to Plug-in Op-Codes
 * These Op-Codes are emitted by the host and we must either handle them or return 0 (false).
 */
enum VST_EFFECT_OPCODE {
	/* Create/Initialize the effect (if it has not been created already).
	 * 
	 * @return Always 0.
	 */
	VST_EFFECT_OPCODE_00         = 0x00,
	VST_EFFECT_OPCODE_CREATE     = 0x00,
	VST_EFFECT_OPCODE_INITIALIZE = 0x00,

	/* Destroy the effect (if there is any) and free its memory.
	 *
	 * This should destroy the actual object created by VST_ENTRYPOINT.
	 * 
	 * @return Always 0.
	 */
	VST_EFFECT_OPCODE_01      = 0x01,
	VST_EFFECT_OPCODE_DESTROY = 0x01,

	/* Set Program
	 *
	 * Set which program number is currently selected.
	 * @param p_int2 The program number to set. Can be negative for some reason.
	 */
	VST_EFFECT_OPCODE_02 = 0x02,
	VST_EFFECT_OPCODE_SET_PROGRAM = 0x02,

	/* Get Program
	 *
	 * Get which program number is currently selected.
	 * @return The currently set program number. Can be negative for some reason.
	 */
	VST_EFFECT_OPCODE_03 = 0x03,
	VST_EFFECT_OPCODE_GET_PROGRAM = 0x03,

	/* Set Program Name
	 *
	 * Set the name of the currently selected program.
	 *
	 * @param p_ptr `const char[VST_BUFFER_SIZE_PROGRAM_NAME]` Zero terminated string.
	 */
	VST_EFFECT_OPCODE_04 = 0x04,
	VST_EFFECT_OPCODE_SET_PROGRAM_NAME = 0x04,

	/* Get Program Name
	 *
	 * Get the name of the currently selected program.
	 * @param p_ptr `char[VST_BUFFER_SIZE_PROGRAM_NAME]` Zero terminated string.
	 */
	VST_EFFECT_OPCODE_05 = 0x05,
	VST_EFFECT_OPCODE_GET_PROGRAM_NAME = 0x05,

	/* Get the value? label for the parameter.
	 * 
	 * @param p_int1 Parameter index.
	 * @param p_ptr 'char[VST_BUFFER_SIZE_PARAM_LABEL]' Zero terminated string.
	 * @return 0 on success, 1 on failure.
	 */
	VST_EFFECT_OPCODE_06             = 0x06,
	VST_EFFECT_OPCODE_PARAM_GETLABEL = 0x06,

	/* Get the string representing the value for the parameter.
	 * 
	 * @param p_int1 Parameter index.
	 * @param p_ptr 'char[VST_BUFFER_SIZE_PARAM_VALUE]' Zero terminated string.
	 * @return 0 on success, 1 on failure.
	 */
	VST_EFFECT_OPCODE_07             = 0x07,
	VST_EFFECT_OPCODE_PARAM_GETVALUE = 0x07,

	/* Get the name for the parameter.
	 * 
	 * @param p_int1 Parameter index.
	 * @param p_ptr 'char[VST_BUFFER_SIZE_PARAM_NAME]' Zero terminated string.
	 * @return 0 on success, 1 on failure.
	 */
	VST_EFFECT_OPCODE_08            = 0x08,
	VST_EFFECT_OPCODE_PARAM_GETNAME = 0x08,

	/* 
	 * 
	 * Note: Doesn't appear in any VST 2.2 or later plugins I checked.
	 */
	VST_EFFECT_OPCODE_09 = 0x09,

	/* Set the new sample rate for the plugin to use.
	 * 
	 * @param p_float New sample rate as a float (double on 64-bit because register upgrades).
	 */
	VST_EFFECT_OPCODE_0A              = 0x0A,
	VST_EFFECT_OPCODE_SETSAMPLERATE   = 0x0A,
	VST_EFFECT_OPCODE_SET_SAMPLE_RATE = 0x0A,

	/* Sets the block size, which is the maximum number of samples passed into the effect via process calls.
	 *
	 * @param p_int2 The maximum number of samples to be passed in.
	 */
	VST_EFFECT_OPCODE_0B             = 0x0B,
	VST_EFFECT_OPCODE_SETBLOCKSIZE   = 0x0B,
	VST_EFFECT_OPCODE_SET_BLOCK_SIZE = 0x0B,

	/* Effect processing should be suspended/paused.
	 *
	 * Unclear if this is should result in a flush of buffers.
	 * 
	 * @param p_int2 0 if the effect should suspend processing, 1 if it should resume.
	 */
	VST_EFFECT_OPCODE_0C      = 0x0C,
	VST_EFFECT_OPCODE_SUSPEND = 0x0C,

	/* Retrieve the client rect size of the plugins window.
	 * If no window has been created, returns the default rect.
	 *
	 * @param p_ptr Pointer of type 'struct vst_rect*'.
	 * @return On success, returns 1 and updates p_ptr to the rect. On failure, returns 0.
	 */
	VST_EFFECT_OPCODE_0D             = 0x0D,
	VST_EFFECT_OPCODE_WINDOW_GETRECT = 0x0D,
	VST_EFFECT_OPCODE_EDITOR_RECT = 0x0D,

	/* Create the window for the plugin.
	 * 
	 * @param p_ptr HWND of the parent window.
	 * @return 0 on failure, or HWND on success.
	 */
	VST_EFFECT_OPCODE_0E            = 0x0E,
	VST_EFFECT_OPCODE_WINDOW_CREATE = 0x0E,
	VST_EFFECT_OPCODE_EDITOR_OPEN = 0x0E,

	/* Destroy the plugins window.
	 * 
	 * @return Always 0.
	 */
	VST_EFFECT_OPCODE_0F             = 0x0F,
	VST_EFFECT_OPCODE_WINDOW_DESTROY = 0x0F,
	VST_EFFECT_OPCODE_EDITOR_CLOSE = 0x0F,

	/* Window Draw Event?
	 *
	 * Ocasionally called simultaneously as WM_DRAW on windows.
	 *
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 */
	VST_EFFECT_OPCODE_10 = 0x10,
	VST_EFFECT_OPCODE_WINDOW_DRAW = 0x10,

	/* Window Mouse Event?
	 *
	 * Called at the same time mouse events happen.
	 * 
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 */
	VST_EFFECT_OPCODE_11 = 0x11,
	VST_EFFECT_OPCODE_WINDOW_MOUSE = 0x11,

	/* Window Keyboard Event?
	 *
	 * Called at the same time keyboard events happen.
	 *
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 */
	VST_EFFECT_OPCODE_12 = 0x12,
	VST_EFFECT_OPCODE_WINDOW_KEYBOARD = 0x12,

	/* Idle/Keep-Alive Callback?
	 * 
	 * Does not receive any parameters. Randomly called when nothing happens? Idle/Keep-Alive callback?
	 *
	 * Note: Not present in many VST 2.4 plugins.
	 */
	VST_EFFECT_OPCODE_13 = 0x13,
	VST_EFFECT_OPCODE_KEEP_ALIVE = 0x13,

	/* Window Focus Event?
	 *
	 * Sometimes called when the editor window goes back into focus.
	 * 
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 */
	VST_EFFECT_OPCODE_14 = 0x14,

	/* Window Unfocus Event?
	 *
	 * Sometimes called when the editor window goes out of focus.
	 *	  
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 */
	VST_EFFECT_OPCODE_15 = 0x15,

	/* 
	 * 
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 * Note: Some plugins always return the FourCC 'NvEf' (0x4E764566), other plugins return nothing.
	 */
	VST_EFFECT_OPCODE_16 = 0x16,
	VST_EFFECT_OPCODE_FOURCC = 0x16,

	/* Get Chunk Data
	 *
	 * Save current program or bank state to a buffer.
	 * Behavior is different based on the chunk flag.
	 * 
	 * @param p_int1	0 means Bank, 1 means Program, nothing else used?
	 * @param p_ptr		`void**` Pointer to a potential pointer containing your own chunk data.
	 * @return 			Size of the Chunk Data in bytes.
	 */
	VST_EFFECT_OPCODE_17 = 0x17,
	VST_EFFECT_OPCODE_GET_CHUNK_DATA = 0x17,

	/* Set Chunk Data
	 *
	 * Restore current program or bank state from a buffer.
	 * Behavior is different based on the chunk flag.
	 * 
	 * @param p_int1	0 means Bank, 1 means Program, nothing else used?
	 * @param p_int2	Size of the Chunk Data in bytes.
	 * @param p_ptr		`void*` Pointer to a buffer containing chunk data.
	 */
	VST_EFFECT_OPCODE_18 = 0x18,
	VST_EFFECT_OPCODE_SET_CHUNK_DATA = 0x18,

	//--------------------------------------------------------------------------------
	// VST 2.x starts here.
	//--------------------------------------------------------------------------------

	/*
	 *
	 * Appears to be related to midi and audio events.
	 */
	VST_EFFECT_OPCODE_19 = 0x19,

	/* Can the parameter be automated?
	 *
	 * @param p_int1 Index of the parameter.
	 * @return 1 if the parameter can be automated, otherwise 0.
	 */
	VST_EFFECT_OPCODE_1A                   = 0x1A,
	VST_EFFECT_OPCODE_PARAM_ISAUTOMATABLE  = 0x1A,
	VST_EFFECT_OPCODE_PARAM_IS_AUTOMATABLE = 0x1A,

	/* Set Parameter value from string representation.
	 *
	 * @param p_int1 Index of the parameter.
	 * @param p_ptr `const char*` Zero terminated string representation of the value to set.
	 * @return 1 if it worked, otherwise 0.
	 */
	VST_EFFECT_OPCODE_1B = 0x1B,
	VST_EFFECT_OPCODE_PARAM_VALUE_FROM_STRING = 0x1B,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_1C = 0x1C,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_1D = 0x1D, // See VST_EFFECT_OPCODE_05

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_1E = 0x1E,

	/* Input connected.
	 *
	 *
	 */
	VST_EFFECT_OPCODE_1F = 0x1F,

	/* Input disconnected.
	 *
	 *
	 */
	VST_EFFECT_OPCODE_20 = 0x20,

	/* Retrieve the name of the input channel at the given index.
	 *
	 * @param p_int1 Index of the input to get the name for.
	 * @param p_ptr Pointer to a char* buffer able to hold at minimum 20 characters. Might need to be 32 even.
	 * @return 0 on failure, 1 on success.
	 */
	VST_EFFECT_OPCODE_21                   = 0x21,
	VST_EFFECT_OPCODE_INPUT_GETCHANNELNAME = 0x21,
	VST_EFFECT_OPCODE_INPUT_CHANNEL_NAME   = 0x21,

	/* Retrieve the name of the output channel at the given index.
	 *
	 * @param p_int1 Index of the output to get the name for.
	 * @param p_ptr Pointer to a char* buffer able to hold at minimum 20 characters. Might need to be 32 even.
	 * @return 0 on failure, 1 on success.
	 */
	VST_EFFECT_OPCODE_22                    = 0x22,
	VST_EFFECT_OPCODE_OUTPUT_GETCHANNELNAME = 0x22,
	VST_EFFECT_OPCODE_OUTPUT_CHANNEL_NAME   = 0x22,

	/* Retrieve category of this effect.
	 *
	 * @return The category that this effect is in, see VST_CATEGORY.
	 */
	VST_EFFECT_OPCODE_23              = 0x23,
	VST_EFFECT_OPCODE_EFFECT_CATEGORY = 0x23,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_24 = 0x24,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_25 = 0x25,

	/*
	 *
	 * Seen in plug-ins with VST_CATEGORY_OFFLINE.
	 */
	VST_EFFECT_OPCODE_26 = 0x26,

	/*
	 *
	 * Seen in plug-ins with VST_CATEGORY_OFFLINE.
	 */
	VST_EFFECT_OPCODE_27 = 0x27,

	/*
	 *
	 * Seen in plug-ins with VST_CATEGORY_OFFLINE.
	 */
	VST_EFFECT_OPCODE_28 = 0x28,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_29 = 0x29,

	/* Set the speaker arrangement 
	 *
	 * @param p_int2 (vst_speaker_arrangement*) Pointer to a pointer to the speaker arrangement for the input.
	 * @param p_ptr (vst_speaker_arrangement*) Pointer to a pointer to the speaker arrangement for the output.
	 */
	VST_EFFECT_OPCODE_2A                      = 0x2A,
	VST_EFFECT_OPCODE_SET_SPEAKER_ARRANGEMENT = 0x2A,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_2B = 0x2B,

	/* Enable/Disable bypassing the effect.
	 *
	 * @param p_int2 Zero if bypassing the effect is disabled, otherwise 1.
	 */
	VST_EFFECT_OPCODE_2C     = 0x2C,
	VST_EFFECT_OPCODE_BYPASS = 0x2C,

	/* Retrieve the effect name into the ptr buffer.
	 *
	 * Bug: Some officially licensed hosts do not provide the expected buffer size! The lowest I've seen is 32 bytes.
	 * 
	 * @param p_ptr `char[VST_BUFFER_SIZE_EFFECT_NAME]` Zero terminated string. 
	 * @return Always 0, even on failure.
	 */
	VST_EFFECT_OPCODE_2D          = 0x2D,
	VST_EFFECT_OPCODE_GETNAME     = 0x2D,
	VST_EFFECT_OPCODE_EFFECT_NAME = 0x2D,

	/* Translate an error code to a string.
	 *
	 * Note: Not called in almost all licensed hosts.
	 * Note: The buffer size varies wildly and there appears to be no common size.
	 * 
	 * @param p_ptr `char[...]` Zero terminated string buffer to which we write our error message.
	 * @return VST_STATUS_TRUE if we could translate the error, VST_STATUS_FALSE if not.
	 */
	VST_EFFECT_OPCODE_2E              = 0x2E,
	VST_EFFECT_OPCODE_TRANSLATE_ERROR = 0x2E,

	/* Retrieve the vendor name into the ptr buffer.
	 *
	 * @param p_ptr `char[VST_BUFFER_SIZE_PRODUCT]` Zero terminated string.
	 */
	VST_EFFECT_OPCODE_2F          = 0x2F,
	VST_EFFECT_OPCODE_GETVENDOR   = 0x2F,
	VST_EFFECT_OPCODE_VENDOR_NAME = 0x2F,

	/* Retrieve the product name into the ptr buffer.
	 *
	 * @param p_ptr `char[VST_BUFFER_SIZE_PRODUCT]` Zero terminated string.
	 */
	VST_EFFECT_OPCODE_30           = 0x30,
	VST_EFFECT_OPCODE_GETNAME2     = 0x30,
	VST_EFFECT_OPCODE_PRODUCT_NAME = 0x30,

	/* Retrieve the vendor version in return value.
	 *
	 * @return Version.
	 */
	VST_EFFECT_OPCODE_31               = 0x31,
	VST_EFFECT_OPCODE_GETVENDORVERSION = 0x31,
	VST_EFFECT_OPCODE_VENDOR_VERSION   = 0x31,

	/* User defined OP Code, for custom interaction.
	 * 
	 */
	VST_EFFECT_OPCODE_32     = 0x32,
	VST_EFFECT_OPCODE_CUSTOM = 0x32,

	/* Test for support of a specific named feature.
	 * 
	 * @param p_ptr Pointer to a zero-terminated buffer containing the feature name.
	 * @return VST_STATUS_YES if the feature is supported, VST_STATUS_NO if the feature is not supported, VST_STATUS_UNKNOWN in all other cases.
	 */
	VST_EFFECT_OPCODE_33       = 0x33,
	VST_EFFECT_OPCODE_SUPPORTS = 0x33,

	/* Number of samples that are at the tail at the end of playback.
	 * 
	 * @return VST_STATUS_UNKNOWN for automatic tail size, VST_STATUS_TRUE for no tail, any other number above 1 for the number of samples the tail has.
	 */
	VST_EFFECT_OPCODE_34             = 0x34,
	VST_EFFECT_OPCODE_GETTAILSAMPLES = 0x34,
	VST_EFFECT_OPCODE_TAIL_SAMPLES   = 0x34,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_35 = 0x35,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_36 = 0x36,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_37 = 0x37,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_38 = 0x38,

	/* Parameter Properties
	 *
	 * @param p_ptr vst_parameter_properties*
	 * @return 1 if supported, otherwise 0.
	 */
	VST_EFFECT_OPCODE_39                       = 0x39,
	VST_EFFECT_OPCODE_GET_PARAMETER_PROPERTIES = VST_EFFECT_OPCODE_39,

	/* Retrieve the VST Version supported.
	 *
	 * @return Return 0 for <2.0, 2 for 2.0, 2100 for 2.1, 2200 for 2.2, 2300 for 2.3, etc.
	 */
	VST_EFFECT_OPCODE_3A          = 0x3A,
	VST_EFFECT_OPCODE_VST_VERSION = 0x3A,

	// VST 2.1 or later

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_3B = 0x3B,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_3C = 0x3C,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_3D = 0x3D,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_3E = 0x3E,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_3F = 0x3F,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_40 = 0x40,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_41 = 0x41,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_42 = 0x42,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_43 = 0x43,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_44 = 0x44,

	// VST 2.3 or later

	/* Retrieve the speaker arrangement.
	 *
	 * @param p_int2 (vst_speaker_arrangement**) Pointer to a pointer to the speaker arrangement for the input.
	 * @param p_ptr (vst_speaker_arrangement**) Pointer to a pointer to the speaker arrangement for the output.
	 */
	VST_EFFECT_OPCODE_45                      = 0x45,
	VST_EFFECT_OPCODE_GET_SPEAKER_ARRANGEMENT = 0x45,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_46 = 0x46,

	/* Begin processing of audio.
	 *
	 * 
	 * 
	 */
	VST_EFFECT_OPCODE_PROCESS_BEGIN = 0x47,

	/* End processing of audio.
	 *
	 * 
	 *
	 */
	VST_EFFECT_OPCODE_PROCESS_END = 0x48,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_49 = 0x49,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4A = 0x4A,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4B = 0x4B,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4C = 0x4C,

	// VST 2.4 or later

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4D = 0x4D,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4E = 0x4E,

	/*
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4F = 0x4F,

	// Highest number of known OPCODE.
	VST_EFFECT_OPCODE_MAX,

	// Pad to force 32-bit number.
	_VST_EFFECT_OPCODE_PAD = 0xFFFFFFFFul,
};

/** Plug-in Effect definition
 */
struct vst_effect {
	int32_t magic_number; // Should always be VST_MAGICNUMBER

	// 64-bit adds 4-byte padding here to align pointers.

	/* Control the VST through an opcode and up to four parameters.
	 * 
	 * @param this Pointer to the effect itself.
	 * @param opcode The opcode to run, see VST_EFFECT_OPCODES.
	 * @param p_int1 Parameter, see VST_EFFECT_OPCODES.
	 * @param p_int2 Parameter, see VST_EFFECT_OPCODES.
	 * @param p_ptr Parameter, see VST_EFFECT_OPCODES.
	 * @param p_float Parameter, see VST_EFFECT_OPCODES.
	 */
	intptr_t(VST_FUNCTION_INTERFACE* control)(vst_effect* pthis, VST_EFFECT_OPCODE opcode, int32_t p_int1, intptr_t p_int2, void* p_ptr, float p_float);

	/* Process the given number of samples in inputs and outputs.
	 *
	 * Different to process_float how? Never seen any difference.
	 * 
	 * @param pthis Pointer to the effect itself.
	 * @param inputs Pointer to an array of 'const float[samples]' with size numInputs.
	 * @param outputs Pointer to an array of 'float[samples]' with size numOutputs.
	 * @param samples Number of samples per channel in inputs.
	 */
	void(VST_FUNCTION_INTERFACE* process)(vst_effect* pthis, const float* const* inputs, float** outputs, int32_t samples);

	/* Updates the value for the parameter at the given index, or does nothing if out of bounds.
	 * 
	 * @param pthis Pointer to the effect itself.
	 * @param index Parameter index.
	 * @param value New value for the parameter.
	 */
	void(VST_FUNCTION_INTERFACE* set_parameter)(vst_effect* pthis, uint32_t index, float value);

	/* Returns the value stored for the parameter at index, or 0 if out of bounds.
	 * 
	 * @param pthis Pointer to the effect itself.
	 * @param index Parameter index.
	 * @return float Value of the parameter.
	 */
	float(VST_FUNCTION_INTERFACE* get_parameter)(vst_effect* pthis, uint32_t index);

	int32_t num_programs; // Number of available programs.
	int32_t num_params; // Number of parameters. All programs must have at least this many inputs.
	int32_t num_inputs; // Number of inputs.
	int32_t num_outputs; // Number of outputs.

	/* Effect Flags
	 *
	 * See: VST_EFFECT_FLAGS
	 */
	int32_t flags;

	// 64-bit adds 4-byte padding here to align pointers.

	void* _unknown_ptr_00[2];

	/* Initial delay before processing of samples can actually begin in Samples.
	 *
	 * Note: The host can modify this at runtime so it is not safe. 
	 * Note: Should be reinitialized when the effect is resumed.
	 */
	int32_t delay;

	int32_t _unknown_int32_00[2]; // Unknown int32_t values.
	float   _unknown_float_00; // Seems to always be 1.0

	void* effect_internal; // Pointer to Plugin internal data
	void* host_internal; // Pointer to Host internal data.

	/* Id of the plugin.
	 *
	 * Due to this not being enough for uniqueness, it should not be used alone for indexing.
	 * Ideally you want to index like this:
	 *     [unique_id][module_name][version][flags]
	 * If any of the checks after unique_id fail, you default to the first possible choice.
	 *
	 * BUG: Some broken hosts rely on this alone to save information about VST plug-ins.
	 */
	int32_t unique_id;

	/* Plugin version
	 * 
	 * Unrelated to the minimum VST Version, but often the same.
	 */
	int32_t version;

	// There is no padding here if everything went right.

	/* Process the given number of single samples in inputs and outputs.
	 *
	 * @param pthis Pointer to the effect itself.
	 * @param inputs Pointer to an array of 'const float[samples]' with size numInputs.
	 * @param outputs Pointer to an array of 'float[samples]' with size numOutputs.
	 * @param samples Number of samples per channel in inputs.
	 */
	void(VST_FUNCTION_INTERFACE* process_float)(vst_effect* pthis, const float* const* inputs, float** outputs, int32_t samples);

	/* Process the given number of double samples in inputs and outputs.
	 *
	 * Note: Not present and not called prior to VST 2.4. 
	 * 
	 * @param pthis Pointer to the effect itself.
	 * @param inputs Pointer to an array of 'const double[samples]' with size numInputs.
	 * @param outputs Pointer to an array of 'double[samples]' with size numOutputs.
	 * @param samples Number of samples per channel in inputs.
	 */
	void(VST_FUNCTION_INTERFACE* process_double)(vst_effect* pthis, const double* const* inputs, double** outputs, int32_t samples);

	// Everything after this is unknown and was present in reacomp-standalone.dll.
	uint8_t _unknown[56]; // 56-bytes of something. Could also just be 52-bytes.
};

/** VST 2.x Entry Point for all platforms
 *
 * Must be present in VST 2.x plug-ins but must not be present in VST 1.x plug-ins.
 *
 * @return A new instance of the VST 2.x effect.
 */
#define VST_ENTRYPOINT \
	vst_effect* VSTPluginMain(vst_host_callback callback)

/** [DEPRECATED] VST 1.x Entry Point for Windows
 *
 * Do not implement in VST 2.1 or later plug-ins!
 * 
 * @return A new instance of the VST 1.x effect.
 */
#define VST_ENTRYPOINT_WINDOWS \
	vst_effect* MAIN(vst_host_callback callback) { return VSTPluginMain(callback); }

/** [DEPRECATED] VST 1.x Entry Point for MacOS
 *
 * Do not implement in VST 2.1 or later plug-ins!
 * 
 * @return A new instance of the VST 1.x effect.
 */
#define VST_ENTRYPOINT_MACOS \
	vst_effect* main_macho(vst_host_callback callback) { return VSTPluginMain(callback); }

#ifdef __cplusplus
}
#endif
#pragma pack(pop)
#endif
