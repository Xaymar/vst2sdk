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
	VST_STATUS_NO = -1,

	_VST_STATUS_PAD = 0xFFFFFFFFul,
};

/** Known Buffer Sizes
 */
enum VST_BUFFER_SIZE {
	VST_BUFFER_SIZE_PARAM_LABEL = 8,
	VST_BUFFER_SIZE_PARAM_NAME = 8,
	VST_BUFFER_SIZE_PARAM_VALUE = 8,
	VST_BUFFER_SIZE_STREAM_LABEL = 8,
	VST_BUFFER_SIZE_CATEGORY_LABEL = 24,
	VST_BUFFER_SIZE_PROGRAM_NAME = 24,
	VST_BUFFER_SIZE_EFFECT_NAME = 32,
	VST_BUFFER_SIZE_PARAM_LONG_NAME = 64,
	VST_BUFFER_SIZE_PRODUCT_NAME = 64,
	VST_BUFFER_SIZE_SPEAKER_NAME = 64,
	VST_BUFFER_SIZE_STREAM_NAME = 64,
	VST_BUFFER_SIZE_VENDOR_NAME = 64,
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
struct vst_rect_t {
	int16_t top;
	int16_t left;
	int16_t bottom;
	int16_t right;
};

enum VST_PARAMETER_FLAG {
	/** Parameter is an on/off switch.
	 *
	 */
	VST_PARAMETER_FLAG_1ls0 = 1 << 0,
	VST_PARAMETER_FLAG_SWITCH = 1,

	/** Parameter limits are set as integers.
	 *
	 */
	VST_PARAMETER_FLAG_1ls1 = 1 << 1,
	VST_PARAMETER_FLAG_INTEGER_LIMITS = 1 << 1,

	/** Parameter uses float steps.
	 *
	 */
	VST_PARAMETER_FLAG_1ls2 = 1 << 2,
	VST_PARAMETER_FLAG_STEP_FLOAT = 1 << 2,

	/** Parameter uses integer steps.
	 *
	 */
	VST_PARAMETER_FLAG_1ls3 = 1 << 3,
	VST_PARAMETER_FLAG_STEP_INT = 1 << 3,

	/** Parameter has an display order index for the default editor.
	 *
	 * Only applies to the default editor.
	 */
	VST_PARAMETER_FLAG_1ls4 = 1 << 4,
	VST_PARAMETER_FLAG_INDEX = 1 << 4,

	/** Parameter has a category for the default editor.
	 *
	 * Only applies to the default editor.
	 */
	VST_PARAMETER_FLAG_1ls5 = 1 << 5,
	VST_PARAMETER_FLAG_CATEGORY = 1 << 5,

	/** Parameter can be gradually increased/decreased.
	 * 
	 * Used for automation only? How?
	 */
	VST_PARAMETER_FLAG_1ls6 = 1 << 6,
	VST_PARAMETER_FLAG_RAMPING = 1 << 6,

	_VST_PARAMETER_FLAG_PAD     = 0xFFFFFFFFul,
};

/** Information about a parameter
 * 
 * Note: Some hosts and plug-ins expect float parameters to be normalized within 0.0 and 1.0.
 */
struct vst_parameter_properties_t {
	/** Float Step value
	 * 
	 * Some hosts and plug-ins expect this to be within 0 and 1.0.
	 * 
	 * Only valid with VST_PARAMETER_FLAG_STEP_FLOAT
	 */
	float step_f32;

	/** Float small step value
	 * This is used for "tiny" changes.
	 * 
	 * Some hosts and plug-ins expect this to be within 0 and 1.0.
	 * 
	 * Only valid with VST_PARAMETER_FLAG_STEP_FLOAT
	 */
	float step_small_f32;

	/** Float large step value
	 * This is used for "huge" changes.
	 * 
	 * Some hosts and plug-ins expect this to be within 0 and 1.0.
	 * 
	 * Only valid with VST_PARAMETER_FLAG_STEP_FLOAT
	 */
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

	/** Minimum Integer value
	 * 
	 * Only valid with VST_PARAMETER_FLAG_INTEGER_LIMITS
	 */
	int32_t  min_value_i32;

	/** Maximum Integer value
	 * 
	 * Only valid with VST_PARAMETER_FLAG_INTEGER_LIMITS
	 */
	int32_t  max_value_i32;

	/** Integer Step value
	 * 
	 * Only valid with VST_PARAMETER_FLAG_STEP_INT
	 */
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

	uint16_t _unknown_00; // Must be set to 0.

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

	float _unknown_00; // Must be set to 0

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
	struct vst_speaker_properties_t speakers[VST_MAX_CHANNELS]; // Array of speaker properties, actual size defined by channels.
};

enum VST_STREAM_FLAG {
	/** Ignored?
	 */
	VST_STREAM_FLAG_1ls0 = 1 << 0,

	/** Stream is in Stereo
	 * 
	 * Can't be used with VST_STREAM_FLAG_USE_TYPE.
	 */
	VST_STREAM_FLAG_1ls1 = 1 << 1,
	VST_STREAM_FLAG_STEREO = 1 << 1,

	/** Stream is defined by VST_SPEAKER_ARRANGEMENT_TYPE
	 * 
	 * Can't be used with VST_STREAM_FLAG_STEREO.
	 */
	VST_STREAM_FLAG_1ls2 = 1 << 2,
	VST_STREAM_FLAG_USE_TYPE = 1 << 2,
};

struct vst_stream_properties_t {
	/** Human-readable name for this stream.
	 */
	char name[VST_BUFFER_SIZE_STREAM_NAME];

	/** Stream flags
	 * Any combination of VST_STREAM_FLAG
	 */
	int32_t flags;

	/** Stream arrangement (optional)
	 * See VST_SPEAKER_ARRANGEMENT_TYPE
	 */
	int32_t type;

	/** Human-readable label for this stream.
	 */
	char label[VST_BUFFER_SIZE_STREAM_LABEL];

	uint8_t _reserved[48]; // 48 bytes of uninitialized data, always.
};

//------------------------------------------------------------------------------------------------------------------------
// VST Host related Things
//------------------------------------------------------------------------------------------------------------------------

struct vst_effect_t; // Pre-define vst_effect_t so we can use it below.

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

	/** Get the currently selected effect id in container plug-ins.
	 * 
	 * Used in combination with VST_EFFECT_CATEGORY_CONTAINER.
	 * 
	 * @return The currently selected unique effect id in this container.
	 */
	VST_HOST_OPCODE_02 = 0x02, // bool cb(0, 0x02, 0, 0, 0);
	VST_HOST_OPCODE_CURRENT_EFFECT_ID = 0x02,

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

	/** Notify the host that numInputs/numOutputs/delay/numParams has changed.
	 * Only supported if the host replies VST_STATUS_TRUE to VST_HOST_OPCODE_SUPPORTS("ioChanged")
	 * 
	 * Note: In VST 2.3 and earlier calling this outside of VST_EFFECT_OPCODE_IDLE may result in a crash.
	 * Note: In VST 2.3 and later this may only be called while between VST_EFFECT_OPCODE_PROCESS_END and VST_EFFECT_OPCODE_BEGIN.
	 * 
	 * @return VST_STATUS_TRUE if supported and handled otherwise VST_STATUS_FALSE.
	 */
	VST_HOST_OPCODE_0E = 0x0E,
	VST_HOST_OPCODE_IO_MODIFIED = 0x0E,

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

	/** Retrieve the vendor name into the ptr buffer.
	 *
	 * @param p_ptr `char[VST_BUFFER_SIZE_VENDOR]` Zero terminated string.
	 */
	VST_HOST_OPCODE_21 = 0x21,
	VST_HOST_OPCODE_VENDOR_NAME = 0x21,

	/** Retrieve the product name into the ptr buffer.
	 *
	 * @param p_ptr `char[VST_BUFFER_SIZE_PRODUCT]` Zero terminated string.
	 */
	VST_HOST_OPCODE_22 = 0x22,
	VST_HOST_OPCODE_PRODUCT_NAME = 0x22,

	/** Retrieve the vendor version in return value.
	 *
	 * @return Version.
	 */
	VST_HOST_OPCODE_23 = 0x23,
	VST_HOST_OPCODE_VENDOR_VERSION = 0x23,

	/** User defined OP Code, for custom interaction.
	 * 
	 */
	VST_HOST_OPCODE_24 = 0x24,
	VST_HOST_OPCODE_CUSTOM = 0x24,

	VST_HOST_OPCODE_25 = 0x25,	

	/** Check if the host supports a certain feature.
	 * 
	 * @param p_ptr `char[...]` Zero terminated string for which feature we want to support.
	 * @return VST_STATUS_TRUE if the feature is supported otherwise VST_STATUS_FALSE.
	 */
	VST_HOST_OPCODE_26 = 0x26,
	VST_HOST_OPCODE_SUPPORTS = 0x26,

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


/** Plug-in to Hostn support checks
 * 
 * Provided as `char* p_ptr` in the VST_EFFECT_OPCODE_SUPPORTS op code.
 * 
 * Harvested via strings command and just checking what plug-ins actually responded to. * 
 */
struct vst_host_supports {
	/** Does the host support modifying input/output/params/delay when programs, banks or parameters are changed?
	 * This only means that the host supports this inside of VST_EFFECT_OPCODE_IDLE (VST 2.3 or earlier) or outside of
	 * a VST_EFFECT_OPCODE_PROCESS_BEGIN and VST_EFFECT_OPCODE_PROCESS_END group.
	 * 
	 * Signals that the host supports the following:
	 * - VST_HOST_OPCODE_IO_MODIFIED
	 * 
	 * @return VST_STATUS_TRUE if it supports it.
	 */
	const char* acceptIOChanges;

	/** Is the host using process begin/end instead of idle?
	 * 
	 * This behavior is the default for all VST 2.4 hosts, but VST 2.3 hosts can opt for either.
	 * @return VST_STATUS_TRUE if it supports it.
	 */
	const char* startStopProcess;

	/** Does the host support container plug-ins?
	 * 
	 * Signals that the host and plug-in support the following:
	 * - VST_HOST_OPCODE_CURRENT_EFFECT_ID
	 * - VST_EFFECT_OPCODE_CONTAINER_NEXT_EFFECT_ID
	 * 
	 * Note: Is shell a reference to Windows shell menus?
	 * 
	 * @return VST_STATUS_TRUE if the host supports it _and_ the current plug-in is a container plug-in.
	 */
	const char* shellCategory;

	const char* sendVstEvents;
	const char* receiveVstEvents;

	const char* sendVstMidiEvent;
	const char* receiveVstMidiEvent;
	const char* sendVstMidiEventFlagIsRealtime;

	const char* sendVstTimeInfo;
	const char* reportConnectionChanges; // Seems related to speakers?
	const char* sizeWindow;
	const char* offline;

	const char* openFileSelector;
	const char* closeFileSelector;
} vst_host_supports = {
	.acceptIOChanges = "acceptIOChanges",
	.startStopProcess = "startStopProcess",
	.shellCategory = "shellCategory",
	.sendVstEvents = "sendVstEvents",
	.receiveVstEvents = "receiveVstEvents",
	.sendVstMidiEvent = "sendVstMidiEvent",
	.receiveVstMidiEvent = "receiveVstMidiEvent",
	.sendVstMidiEventFlagIsRealtime = "sendVstMidiEventFlagIsRealtime",
	.sendVstTimeInfo = "sendVstTimeInfo",
	.reportConnectionChanges = "reportConnectionChanges",
	.sizeWindow = "sizeWindow",
	.offline = "offline",
	.openFileSelector = "openFileSelector",
	.closeFileSelector = "closeFileSelector",
};

/** Plug-in to Host callback
 *
 * The plug-in may call this to attempt to change things on the host side. The host side is free to ignore all requests, annoyingly enough.
 * 
 * @param opcode See VST_HOST_OPCODE
 * @param p_str Zero terminated string or null on call.
 * @return ?
 */
typedef intptr_t (*vst_host_callback_t)(struct vst_effect_t* plugin, int32_t opcode, int32_t p_int1, int64_t p_int2, const char* p_str, float p_float);

//------------------------------------------------------------------------------------------------------------------------
// VST Plug-in/Effect related Things
//------------------------------------------------------------------------------------------------------------------------

#define VST_MAGICNUMBER 'VstP'
#define VST_DEFAULT_SAMPLE_RATE 44100.0f
#define VST_DEFAULT_BLOCK_SIZE 1024

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

	/** Plug-in contains multiple effects.
	 * 
	 * Host handling:
	 * ```
	 * uint32_t current_select_id;
	 * 
	 * // ... in intptr_t vst_host_callback(vst_effect_t* plugin, VST_HOST_OPCODE opcode, ...)
	 *     case VST_HOST_OPCODE_SUPPORTS: {
	 *       char* text = (char*)p_ptr;
	 *       // The plug-in may ask the host if it even supports containers at all and changes behavior if we don't.
	 *       if (text && strcmp(text, vst_host_supports.shellCategory) == 0) {
	 *         return VST_STATUS_TRUE;
	 *       }
	 *     }
	 *     case VST_HOST_OPCODE_CURRENT_EFFECT_ID:
	 *       return current_selected_id;
	 * // ...
	 * 
	 * // ... in whatever you use to load plug-ins ...
	 *   current_select_id;
	 *   vst_effect_t* plugin = plugin_main(&vst_host_callback);
	 *   int32_t plugin_category = plugin->control(plugin, VST_EFFECT_OPCODE_CATEGORY, 0, 0, 0, 0)
	 *   if (plugin_category == VST_EFFECT_CATEGORY_CONTAINER) {
	 *     char effect_name[VST_BUFFER_SIZE_EFFECT_NAME] effect_name;
	 *     int32_t effect_id;
	 *     // Iterate over all contained effects.
	 *     while ((effect_id = plugin->control(plugin, VST_EFFECT_OPCODE_CONTAINER_NEXT_EFFECT_ID, 0, 0, effect_name, 0)) != 0) {
	 *       // Contained effects must be named as far as I can tell.
	 *       if (effect_name[0] != 0) {
	 *          // Do some logic that does the necessary things to list these in the host.
	 *       }
	 *     }
	 *   } else {
	 *     // Do things to list only this plugin in the host.
	 *   }
	 * // ...
	 * ```
	 * 
	 * Plug-in handling:
	 * ```
	 * 
	 * // ... in vst_effect for the container
	 *   size_t current_effect_idx;
	 *   int32_t effect_list[] = {
	 *     // ... list of effect ids.
	 *   }
	 * // ... in control(...)
	 *     case VST_EFFECT_OPCODE_CONTAINER_NEXT_EFFECT_ID:
	 *       // Make sure current_effect_idx doesn't exceed the maximum.
	 *       if (current_effect_idx > ARRAYSIZEOF(effect_list)) {
	 *         current_effect_idx;
	 *         return 0;
	 *       }
	 *       // Some code that turns effect indices into names to store into p_ptr.
	 *       return effect_list[current_effect_idx++]; // Return the effect id.
	 * // ...
	 * 
	 * VST_ENTRYPOINT {
	 *   // Ensure the host VST 2.x compatible.
	 *   int32_t vst_version = callback(nullptr, VST_HOST_OPCODE_VST_VERSION, 0, 0, 0, 0);
	 *   if (vst_version == 0) {
	 *     return 0; // It's not so we exit early.
	 *   }
	 *   
	 *   // Check if the host wants 
	 *   int32_t effect_id = callback(nullptr, VST_HOST_OPCODE_CURRENT_EFFECT_ID, 0, 0, 0);
	 *   if (effect_id == 0) {
	 *     // ... logic specific to making the container.
	 *     return new vst_container_effect();
	 *   } else {
	 *     // ... logic specific to make sub effects
	 *     return new vst_sub_effect();
	 *   }
	 * }
	 * 
	 * // ...
	 * ```
	 */
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
	/** Effect provides a custom editor.
	 * The host will not provide a generic editor interface and expects VST_EFFECT_OPCODE_EDITOR_OPEN/_CLOSE to work as
	 * expected. We are in charge of notifying the host about various things like which parameter is in focus and stuff.
	 */
	VST_EFFECT_FLAG_EDITOR = 1 << 0, // The plug-in provides a custom editor instead of the generic host interface.

	//1 << 1,
	//1 << 2, // Only seen when the plug-in responds to VST_EFFECT_OPCODE_09. Seems to be ignored by hosts entirely.
	//1 << 3, // Only seen when the plug-in behaves differently in mono mode. Seems to be ignored by hosts entirely.

	/** Effect uses process_float.
	 * 
	 * This is the default behavior for VST 2.4 and must always be set if making a VST 2.4 plug-in.
	 */
	VST_EFFECT_FLAG_SUPPORTS_FLOAT = 1 << 4,

	/** Effect supports saving/loading programs/banks from unformatted chunk data.
	 * When not set some sort of format is expected that I've yet to decipher.
	 * 
	 */
	VST_EFFECT_FLAG_CHUNKS = 1 << 5,

	//1 << 6,
	//1 << 7,
	//1 << 8,

	/** Effect is an Instrument/Generator
	 */
	VST_EFFECT_FLAG_INSTRUMENT = 1 << 8, 

	//1 << 9,
	
	/** Effect does not produce tail samples when the input is silent.
	 * 
	 * Not to be confused with choosing to tell the host there is no tail.
	 */
	VST_EFFECT_FLAG_SILENT_TAIL = 1 << 9,

	//1 << 10,
	//1 << 11,
	//1 << 12,
	
	/** Effect supports process_double.
	 * The host can freely choose between process_float and process_double as required.
	 * 
	 * Only available for VST 2.4 compatible plug-ins and hosts.
	 */
	VST_EFFECT_FLAG_SUPPORTS_DOUBLE = 1 << 12,
};

/** Host to Plug-in Op-Codes
 * These Op-Codes are emitted by the host and we must either handle them or return 0 (false).
 */
enum VST_EFFECT_OPCODE {
	/** Create/Initialize the effect (if it has not been created already).
	 * 
	 * @return Always 0.
	 */
	VST_EFFECT_OPCODE_00         = 0x00,
	VST_EFFECT_OPCODE_CREATE     = 0x00,
	VST_EFFECT_OPCODE_INITIALIZE = 0x00,

	/** Destroy the effect (if there is any) and free its memory.
	 *
	 * This should destroy the actual object created by VST_ENTRYPOINT.
	 * 
	 * @return Always 0.
	 */
	VST_EFFECT_OPCODE_01      = 0x01,
	VST_EFFECT_OPCODE_DESTROY = 0x01,

	/** Set Program
	 *
	 * Set which program number is currently selected.
	 * @param p_int2 The program number to set. Can be negative for some reason.
	 */
	VST_EFFECT_OPCODE_02 = 0x02,
	VST_EFFECT_OPCODE_SET_PROGRAM = 0x02,
	VST_EFFECT_OPCODE_PROGRAM_SET = 0x02,

	/** Get Program
	 *
	 * Get which program number is currently selected.
	 * @return The currently set program number. Can be negative for some reason.
	 */
	VST_EFFECT_OPCODE_03 = 0x03,
	VST_EFFECT_OPCODE_GET_PROGRAM = 0x03,
	VST_EFFECT_OPCODE_PROGRAM_GET = 0x03,

	/** Set Program Name
	 *
	 * Set the name of the currently selected program.
	 *
	 * @param p_ptr `const char[VST_BUFFER_SIZE_PROGRAM_NAME]` Zero terminated string.
	 */
	VST_EFFECT_OPCODE_04 = 0x04,
	VST_EFFECT_OPCODE_SET_PROGRAM_NAME = 0x04,
	VST_EFFECT_OPCODE_PROGRAM_SET_NAME = 0x04,

	/** Get Program Name
	 *
	 * Get the name of the currently selected program.
	 * @param p_ptr `char[VST_BUFFER_SIZE_PROGRAM_NAME]` Zero terminated string.
	 */
	VST_EFFECT_OPCODE_05 = 0x05,
	VST_EFFECT_OPCODE_GET_PROGRAM_NAME = 0x05,
	VST_EFFECT_OPCODE_PROGRAM_GET_NAME = 0x05,

	/** Get the value? label for the parameter.
	 * 
	 * @param p_int1 Parameter index.
	 * @param p_ptr 'char[VST_BUFFER_SIZE_PARAM_LABEL]' Zero terminated string.
	 * @return 0 on success, 1 on failure.
	 */
	VST_EFFECT_OPCODE_06             = 0x06,
	VST_EFFECT_OPCODE_PARAM_GETLABEL = 0x06,
	VST_EFFECT_OPCODE_PARAM_GET_LABEL = 0x06,

	/** Get the string representing the value for the parameter.
	 * 
	 * @param p_int1 Parameter index.
	 * @param p_ptr 'char[VST_BUFFER_SIZE_PARAM_VALUE]' Zero terminated string.
	 * @return 0 on success, 1 on failure.
	 */
	VST_EFFECT_OPCODE_07             = 0x07,
	VST_EFFECT_OPCODE_PARAM_GETVALUE = 0x07,
	VST_EFFECT_OPCODE_PARAM_GET_VALUE = 0x07,

	/** Get the name for the parameter.
	 * 
	 * @param p_int1 Parameter index.
	 * @param p_ptr 'char[VST_BUFFER_SIZE_PARAM_NAME]' Zero terminated string.
	 * @return 0 on success, 1 on failure.
	 */
	VST_EFFECT_OPCODE_08            = 0x08,
	VST_EFFECT_OPCODE_PARAM_GETNAME = 0x08,
	VST_EFFECT_OPCODE_PARAM_GET_NAME = 0x08,

	/** 
	 * 
	 * Note: Doesn't appear in any VST 2.2 or later plugins I checked.
	 */
	VST_EFFECT_OPCODE_09 = 0x09,

	/** Set the new sample rate for the plugin to use.
	 * 
	 * @param p_float New sample rate as a float (double on 64-bit because register upgrades).
	 */
	VST_EFFECT_OPCODE_0A              = 0x0A,
	VST_EFFECT_OPCODE_SETSAMPLERATE   = 0x0A,
	VST_EFFECT_OPCODE_SET_SAMPLE_RATE = 0x0A,

	/** Sets the block size, which is the maximum number of samples passed into the effect via process calls.
	 *
	 * @param p_int2 The maximum number of samples to be passed in.
	 */
	VST_EFFECT_OPCODE_0B             = 0x0B,
	VST_EFFECT_OPCODE_SETBLOCKSIZE   = 0x0B,
	VST_EFFECT_OPCODE_SET_BLOCK_SIZE = 0x0B,

	/** Effect processing should be suspended/paused or resumed/unpaused.
	 *
	 * Unclear if this is should result in a flush of buffers. In VST 2.3+ this is quite clear as we get process 
	 * begin/end.
	 * 
	 * @param p_int2 VST_STATUS_FALSE if the effect should suspend processing, VST_STATUS_TRUE if it should resume.
	 */
	VST_EFFECT_OPCODE_0C      = 0x0C,
	VST_EFFECT_OPCODE_PAUSE_UNPAUSE = 0x0C,
	VST_EFFECT_OPCODE_SUSPEND_RESUME = 0x0C,
	VST_EFFECT_OPCODE_SUSPEND = 0x0C,

	/** Retrieve the client rect size of the plugins window.
	 * If no window has been created, returns the default rect.
	 *
	 * @param p_ptr Pointer of type 'struct vst_rect_t*'.
	 * @return On success, returns 1 and updates p_ptr to the rect. On failure, returns 0.
	 */
	VST_EFFECT_OPCODE_0D             = 0x0D,
	VST_EFFECT_OPCODE_WINDOW_GETRECT = 0x0D,
	VST_EFFECT_OPCODE_EDITOR_RECT = 0x0D,
	VST_EFFECT_OPCODE_EDITOR_GET_RECT = 0x0D,

	/** Create the window for the plugin.
	 * 
	 * @param p_ptr HWND of the parent window.
	 * @return 0 on failure, or HWND on success.
	 */
	VST_EFFECT_OPCODE_0E            = 0x0E,
	VST_EFFECT_OPCODE_WINDOW_CREATE = 0x0E,
	VST_EFFECT_OPCODE_EDITOR_OPEN = 0x0E,

	/** Destroy the plugins window.
	 * 
	 * @return Always 0.
	 */
	VST_EFFECT_OPCODE_0F             = 0x0F,
	VST_EFFECT_OPCODE_WINDOW_DESTROY = 0x0F,
	VST_EFFECT_OPCODE_EDITOR_CLOSE = 0x0F,

	/** Window Draw Event?
	 *
	 * Ocasionally called simultaneously as WM_DRAW on windows.
	 *
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 * Note: Only on Mac OS?
	 */
	VST_EFFECT_OPCODE_10 = 0x10,
	VST_EFFECT_OPCODE_WINDOW_DRAW = 0x10,
	VST_EFFECT_OPCODE_EDITOR_DRAW = 0x10,

	/** Window Mouse Event?
	 *
	 * Called at the same time mouse events happen.
	 * 
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 * Note: Only on Mac OS?
	 */
	VST_EFFECT_OPCODE_11 = 0x11,
	VST_EFFECT_OPCODE_WINDOW_MOUSE = 0x11,
	VST_EFFECT_OPCODE_EDITOR_MOUSE = 0x11,

	/** Window Keyboard Event?
	 *
	 * Called at the same time keyboard events happen.
	 *
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 * Note: Only on Mac OS?
	 */
	VST_EFFECT_OPCODE_12 = 0x12,
	VST_EFFECT_OPCODE_WINDOW_KEYBOARD = 0x12,
	VST_EFFECT_OPCODE_EDITOR_KEYBOARD = 0x12,

	/** Window/Editor Idle/Keep-Alive Callback?
	 * 
	 * Does not receive any parameters. Randomly called when nothing happens? Idle/Keep-Alive callback?
	 *
	 * Note: Not present in many VST 2.4 plugins.
	 */
	VST_EFFECT_OPCODE_13 = 0x13,
	VST_EFFECT_OPCODE_EDITOR_KEEP_ALIVE = 0x13,

	/** Window Focus Event?
	 *
	 * Sometimes called when the editor window goes back into focus.
	 * 
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 * Note: Only on Mac OS?
	 */
	VST_EFFECT_OPCODE_14 = 0x14,

	/** Window Unfocus Event?
	 *
	 * Sometimes called when the editor window goes out of focus.
	 *	  
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 * Note: Only on Mac OS?
	 */
	VST_EFFECT_OPCODE_15 = 0x15,

	/** 
	 * 
	 * Note: Present in some VST 2.1 or earlier plugins.
	 * Note: Not present in many VST 2.4 plugins.
	 * Note: Some plugins always return the FourCC 'NvEf' (0x4E764566), other plugins return nothing.
	 */
	VST_EFFECT_OPCODE_16 = 0x16,
	VST_EFFECT_OPCODE_FOURCC = 0x16,

	/** Get Chunk Data
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

	/** Set Chunk Data
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

	/**
	 *
	 * Appears to be related to midi and audio events.
	 */
	VST_EFFECT_OPCODE_19 = 0x19,

	/** Can the parameter be automated?
	 *
	 * @param p_int1 Index of the parameter.
	 * @return 1 if the parameter can be automated, otherwise 0.
	 */
	VST_EFFECT_OPCODE_1A                   = 0x1A,
	VST_EFFECT_OPCODE_PARAM_ISAUTOMATABLE  = 0x1A,
	VST_EFFECT_OPCODE_PARAM_IS_AUTOMATABLE = 0x1A,

	/** Set Parameter value from string representation.
	 *
	 * @param p_int1 Index of the parameter.
	 * @param p_ptr `const char*` Zero terminated string representation of the value to set.
	 * @return 1 if it worked, otherwise 0.
	 */
	VST_EFFECT_OPCODE_1B = 0x1B,
	VST_EFFECT_OPCODE_PARAM_VALUE_FROM_STRING = 0x1B,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_1C = 0x1C,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_1D = 0x1D, // See VST_EFFECT_OPCODE_05

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_1E = 0x1E,

	/** Input connected.
	 *
	 *
	 */
	VST_EFFECT_OPCODE_1F = 0x1F,

	/** Input disconnected.
	 *
	 *
	 */
	VST_EFFECT_OPCODE_20 = 0x20,

	/** Retrieve properties for the given input index.
	 *
	 * @param p_int1 Index of the input to get the properties for.
	 * @param p_ptr `vst_stream_properties_t*` Pointer to the properties structure for the selected input provided by the host.
	 * @return VST_STATUS_TRUE if p_ptr is updated, VST_STATUS_FALSE otherwise.
	 */
	VST_EFFECT_OPCODE_21                   = 0x21,
	VST_EFFECT_OPCODE_INPUT_GET_PROPERTIES = 0x21,
	//VST_EFFECT_OPCODE_INPUT_GETCHANNELNAME = 0x21,
	//VST_EFFECT_OPCODE_INPUT_CHANNEL_NAME   = 0x21,

	/** Retrieve properties for the given output index.
	 *
	 * @param p_int1 Index of the output to get the properties for.
	 * @param p_ptr `vst_stream_properties_t*` Pointer to the properties structure for the selected output provided by the host.
	 * @return VST_STATUS_TRUE if p_ptr is updated, VST_STATUS_FALSE otherwise.
	 */
	VST_EFFECT_OPCODE_22                    = 0x22,
	VST_EFFECT_OPCODE_OUTPUT_GET_PROPERTIES = 0x22,
	//VST_EFFECT_OPCODE_OUTPUT_GETCHANNELNAME = 0x22,
	//VST_EFFECT_OPCODE_OUTPUT_CHANNEL_NAME   = 0x22,

	/** Retrieve category of this effect.
	 *
	 * @return The category that this effect is in, see VST_CATEGORY.
	 */
	VST_EFFECT_OPCODE_23              = 0x23,
	VST_EFFECT_OPCODE_EFFECT_CATEGORY = 0x23,
	VST_EFFECT_OPCODE_CATEGORY = 0x23,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_24 = 0x24,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_25 = 0x25,

	/**
	 *
	 * Seen in plug-ins with VST_CATEGORY_OFFLINE.
	 */
	VST_EFFECT_OPCODE_26 = 0x26,

	/**
	 *
	 * Seen in plug-ins with VST_CATEGORY_OFFLINE.
	 */
	VST_EFFECT_OPCODE_27 = 0x27,

	/**
	 *
	 * Seen in plug-ins with VST_CATEGORY_OFFLINE.
	 */
	VST_EFFECT_OPCODE_28 = 0x28,

	/**
	 *
	 * Seen in plug-ins with VST_CATEGORY_OFFLINE.
	 */
	VST_EFFECT_OPCODE_29 = 0x29,

	/** Set the speaker arrangement 
	 *
	 * @param p_int2 (vst_speaker_arrangement*) Pointer to a pointer to the speaker arrangement for the input.
	 * @param p_ptr (vst_speaker_arrangement*) Pointer to a pointer to the speaker arrangement for the output.
	 */
	VST_EFFECT_OPCODE_2A                      = 0x2A,
	VST_EFFECT_OPCODE_SET_SPEAKER_ARRANGEMENT = 0x2A,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_2B = 0x2B,

	/** Enable/Disable bypassing the effect.
	 *
	 * See VST_EFFECT_OPCODE_SUPPORTS with vst_effect_supports.bypass for more information.
	 * 
	 * @param p_int2 Zero if bypassing the effect is disabled, otherwise 1.
	 */
	VST_EFFECT_OPCODE_2C     = 0x2C,
	VST_EFFECT_OPCODE_BYPASS = 0x2C,

	/** Retrieve the effect name into the ptr buffer.
	 *
	 * Bug: Some officially licensed hosts do not provide the expected buffer size! The lowest I've seen is 32 bytes.
	 * 
	 * @param p_ptr `char[VST_BUFFER_SIZE_EFFECT_NAME]` Zero terminated string. 
	 * @return Always 0, even on failure.
	 */
	VST_EFFECT_OPCODE_2D          = 0x2D,
	VST_EFFECT_OPCODE_GETNAME     = 0x2D,
	VST_EFFECT_OPCODE_EFFECT_NAME = 0x2D,

	/** Translate an error code to a string.
	 *
	 * Note: Not called in almost all licensed hosts.
	 * Note: The buffer size varies wildly and there appears to be no common size.
	 * 
	 * @param p_ptr `char[...]` Zero terminated string buffer to which we write our error message.
	 * @return VST_STATUS_TRUE if we could translate the error, VST_STATUS_FALSE if not.
	 */
	VST_EFFECT_OPCODE_2E              = 0x2E,
	VST_EFFECT_OPCODE_TRANSLATE_ERROR = 0x2E,

	/** Retrieve the vendor name into the ptr buffer.
	 *
	 * @param p_ptr `char[VST_BUFFER_SIZE_VENDOR]` Zero terminated string.
	 */
	VST_EFFECT_OPCODE_2F          = 0x2F,
	VST_EFFECT_OPCODE_GETVENDOR   = 0x2F,
	VST_EFFECT_OPCODE_VENDOR_NAME = 0x2F,

	/** Retrieve the product name into the ptr buffer.
	 *
	 * @param p_ptr `char[VST_BUFFER_SIZE_PRODUCT]` Zero terminated string.
	 */
	VST_EFFECT_OPCODE_30           = 0x30,
	VST_EFFECT_OPCODE_GETNAME2     = 0x30,
	VST_EFFECT_OPCODE_PRODUCT_NAME = 0x30,

	/** Retrieve the vendor version in return value.
	 *
	 * @return Version.
	 */
	VST_EFFECT_OPCODE_31               = 0x31,
	VST_EFFECT_OPCODE_GETVENDORVERSION = 0x31,
	VST_EFFECT_OPCODE_VENDOR_VERSION   = 0x31,

	/** User defined OP Code, for custom interaction.
	 * 
	 */
	VST_EFFECT_OPCODE_32     = 0x32,
	VST_EFFECT_OPCODE_CUSTOM = 0x32,

	/** Test for support of a specific named feature.
	 * 
	 * @param p_ptr Pointer to a zero-terminated buffer containing the feature name.
	 * @return VST_STATUS_YES if the feature is supported, VST_STATUS_NO if the feature is not supported, 
	 *         VST_STATUS_UNKNOWN in all other cases.
	 */
	VST_EFFECT_OPCODE_33       = 0x33,
	VST_EFFECT_OPCODE_SUPPORTS = 0x33,

	/** Number of samples that are at the tail at the end of playback.
	 * 
	 * @return VST_STATUS_UNKNOWN for automatic tail size, VST_STATUS_TRUE for no tail, any other number above 1 for 
	 *         the number of samples the tail has.
	 */
	VST_EFFECT_OPCODE_34             = 0x34,
	VST_EFFECT_OPCODE_GETTAILSAMPLES = 0x34,
	VST_EFFECT_OPCODE_TAIL_SAMPLES   = 0x34,

	/** Notify effect that it is idle?
	 *
	 * Note: Invalid/Unused in VST 2.4 as it uses VST_EFFECT_OPCODE_PROCESS_BEGIN and VST_EFFECT_OPCODE_PROCESS_END.
	 */
	VST_EFFECT_OPCODE_35 = 0x35,
	VST_EFFECT_OPCODE_IDLE = 0x35,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_36 = 0x36,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_37 = 0x37,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_38 = 0x38,

	/** Parameter Properties
	 *
	 * @param p_ptr vst_parameter_properties*
	 * @return 1 if supported, otherwise 0.
	 */
	VST_EFFECT_OPCODE_39                       = 0x39,
	VST_EFFECT_OPCODE_GET_PARAMETER_PROPERTIES = VST_EFFECT_OPCODE_39,

	/** Retrieve the VST Version supported.
	 *
	 * @return Return 0 for <2.0, 2 for 2.0, 2100 for 2.1, 2200 for 2.2, 2300 for 2.3, etc.
	 */
	VST_EFFECT_OPCODE_3A          = 0x3A,
	VST_EFFECT_OPCODE_VST_VERSION = 0x3A,

	//--------------------------------------------------------------------------------
	// VST 2.1
	//--------------------------------------------------------------------------------

	/** Editor Virtual Key Down Input
	 *
	 * @param p_int1 ASCII Character
	 * @param p_int2 Virtual Key Code
	 * @param p_float Modifiers
	 * @return VST_STATUS_TRUE if we used the input, otherwise VST_STATUS_FALSE
	 */
	VST_EFFECT_OPCODE_3B = 0x3B,
	VST_EFFECT_OPCODE_EDITOR_VKEY_DOWN = 0x3B,

	/** Editor Virtual Key Up Event
	 *
	 * @param p_int1 ASCII Character
	 * @param p_int2 Virtual Key Code
	 * @param p_float Modifiers
	 * @return VST_STATUS_TRUE if we used the input, otherwise VST_STATUS_FALSE
	 */
	VST_EFFECT_OPCODE_3C = 0x3C,
	VST_EFFECT_OPCODE_EDITOR_VKEY_UP = 0x3C,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_3D = 0x3D,

	/**
	 *
	 * Midi related
	 */
	VST_EFFECT_OPCODE_3E = 0x3E,

	/**
	 *
	 * Midi related
	 */
	VST_EFFECT_OPCODE_3F = 0x3F,

	/**
	 *
	 * Midi related
	 */
	VST_EFFECT_OPCODE_40 = 0x40,

	/**
	 *
	 * Midi related
	 */
	VST_EFFECT_OPCODE_41 = 0x41,

	/**
	 *
	 * Midi related
	 */
	VST_EFFECT_OPCODE_42 = 0x42,

	/** Emitted prior to the host loading a program.
	 *
	 * @return VST_STATUS_TRUE if we understood the notification, or VST_STATUS_FALSE if not.
	 */
	VST_EFFECT_OPCODE_PROGRAM_SET_BEGIN = 0x43,

	/** Emitted after the host finished loading a program.
	 *
	 * @return VST_STATUS_TRUE if we understood the notification, or VST_STATUS_FALSE if not.
	 */
	VST_EFFECT_OPCODE_PROGRAM_SET_END = 0x44,

	// VST 2.3 or later

	/** Retrieve the speaker arrangement.
	 *
	 * @param p_int2 (vst_speaker_arrangement**) Pointer to a pointer to the speaker arrangement for the input.
	 * @param p_ptr (vst_speaker_arrangement**) Pointer to a pointer to the speaker arrangement for the output.
	 */
	VST_EFFECT_OPCODE_45                      = 0x45,
	VST_EFFECT_OPCODE_GET_SPEAKER_ARRANGEMENT = 0x45,

	/** Get the next effect contained in this effect.
	 * This returns the next effect based on an effect internal counter, the host does not provide any index.
	 * 
	 * Used in combination with VST_EFFECT_CATEGORY_CONTAINER.
	 * 
	 * @param p_ptr `char[VST_BUFFER_SIZE_EFFECT_NAME]` Buffer for next effect name.
	 * @return Next effects unique_id
	 */
	VST_EFFECT_OPCODE_46 = 0x46,
	VST_EFFECT_OPCODE_CONTAINER_NEXT_EFFECT_ID = 0x46,

	/** Begin processing of audio.
	 *
	 * 
	 * 
	 */
	VST_EFFECT_OPCODE_47 = 0x47,
	VST_EFFECT_OPCODE_PROCESS_BEGIN = 0x47,

	/** End processing of audio.
	 *
	 * 
	 *
	 */
	VST_EFFECT_OPCODE_48 = 0x48,
	VST_EFFECT_OPCODE_PROCESS_END = 0x48,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_49 = 0x49,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4A = 0x4A,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4B = 0x4B,

	/** Emitted prior to loading a program
	 *
	 * @param p_ptr Unknown structured data.
	 * @return VST_STATUS_NO if we couldn't load the data, VST_STATUS_YES if we can load the data, VST_STATUS_UNKNOWN if this isn't supported.
	 */
	VST_EFFECT_OPCODE_4C = 0x4C,
	VST_EFFECT_OPCODE_PROGRAM_LOAD = 0x4C,

	// VST 2.4 or later

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4D = 0x4D,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4E = 0x4E,

	/**
	 *
	 *
	 */
	VST_EFFECT_OPCODE_4F = 0x4F,

	// Highest number of known OPCODE.
	VST_EFFECT_OPCODE_MAX,

	// Pad to force 32-bit number.
	_VST_EFFECT_OPCODE_PAD = 0xFFFFFFFFul,
};

/** Host to Plug-in support checks
 * 
 * Provided as `char* p_ptr` in the VST_EFFECT_OPCODE_SUPPORTS op code.
 * 
 * Harvested via strings command and just checking what plug-ins actually responded to. * 
 */
struct {
	/** Effect supports alternative bypass.
	 * The alternative bypass still has the host call process/process_float/process_double and expects us to compensate
	 * for our delay/latency, copy inputs to outputs, and do minimal work. If we don't support it the host will not call
	 * process/process_float/process_double at all while bypass is enabled.
	 *
	 * Note: VST 2.3 or later only.
	 * @return VST_STATUS_TRUE if we support this, otherwise VST_STATUS_FALSE.
	 */
	const char* bypass;

	const char* sendVstEvents;
	const char* receiveVstEvents;
	const char* sendVstMidiEvent;
	const char* receiveVstMidiEvent;
	const char* midiProgramNames; // VST 2.1 or later.
	const char* receiveVstTimeInfo;
	const char* offline;
	// The following were only found in VST 2.3 plug-ins	
	const char* plugAsChannelInsert;
	const char* conformsToWindowRules; // Mac OS only, invalid in VST 2.4. Seems related to vst_host_supports.sizeWindow
	const char* plugAsSend;
	const char* mixDryWet;
	const char* noRealTime;
	const char* multipass;
	const char* metapass;
	const char* _1in1out;
	const char* _1in2out;
	const char* _2in1out;
	const char* _2in2out;
	const char* _2in4out;
	const char* _4in2out;
	const char* _4in4out;
	const char* _4in8out;
	const char* _8in4out;
	const char* _8in8out;
} vst_effect_supports = {
	.bypass = "bypass",
	.sendVstEvents = "sendVstEvents",
	.receiveVstEvents = "receiveVstEvents",
	.sendVstMidiEvent = "sendVstMidiEvent",
	.receiveVstMidiEvent = "receiveVstMidiEvent",
	.midiProgramNames = "midiProgramNames",
	.receiveVstTimeInfo = "receiveVstTimeInfo",
	.offline = "offline",
	.plugAsChannelInsert = "plugAsChannelInsert",
	.conformsToWindowRules = "conformsToWindowRules",
	.plugAsSend = "plugAsSend",
	.mixDryWet = "mixDryWet",
	.noRealTime = "noRealTime",
	.multipass = "multipass",
	.metapass = "metapass",
	._1in1out = "1in1out",
	._1in2out = "1in2out",
	._2in1out = "2in1out",
	._2in2out = "2in2out",
	._2in4out = "2in4out",
	._4in2out = "4in2out",
	._4in4out = "4in4out",
	._4in8out = "4in8out",
	._8in4out = "8in4out",
	._8in8out = "8in8out",
};

/** Plug-in Effect definition
 */
struct vst_effect_t {
	int32_t magic_number; // Should always be VST_MAGICNUMBER ('VstP')

	// 64-bit adds 4-byte padding here to align pointers.

	/** Control the VST through an opcode and up to four parameters.
	 * 
	 * @param this Pointer to the effect itself.
	 * @param opcode The opcode to run, see VST_EFFECT_OPCODES.
	 * @param p_int1 Parameter, see VST_EFFECT_OPCODES.
	 * @param p_int2 Parameter, see VST_EFFECT_OPCODES.
	 * @param p_ptr Parameter, see VST_EFFECT_OPCODES.
	 * @param p_float Parameter, see VST_EFFECT_OPCODES.
	 */
	intptr_t(VST_FUNCTION_INTERFACE* control)(struct vst_effect_t* pthis, int32_t opcode, int32_t p_int1, intptr_t p_int2, void* p_ptr, float p_float);

	/** Process the given number of samples in inputs and outputs.
	 *
	 * Used to handle input data and provides output data. We seem to be the ones that provide the output buffer?
	 * 
	 * @param pthis Pointer to the effect itself.
	 * @param inputs Pointer to an array of 'const float[samples]' with size numInputs.
	 * @param outputs Pointer to an array of 'float[samples]' with size numOutputs.
	 * @param samples Number of samples per channel in inputs.
	 */
	void(VST_FUNCTION_INTERFACE* process)(struct vst_effect_t* pthis, const float* const* inputs, float** outputs, int32_t samples);

	/** Updates the value for the parameter at the given index, or does nothing if out of bounds.
	 * 
	 * @param pthis Pointer to the effect itself.
	 * @param index Parameter index.
	 * @param value New value for the parameter.
	 */
	void(VST_FUNCTION_INTERFACE* set_parameter)(struct vst_effect_t* pthis, uint32_t index, float value);

	/** Returns the value stored for the parameter at index, or 0 if out of bounds.
	 * 
	 * @param pthis Pointer to the effect itself.
	 * @param index Parameter index.
	 * @return float Value of the parameter.
	 */
	float(VST_FUNCTION_INTERFACE* get_parameter)(struct vst_effect_t* pthis, uint32_t index);

	int32_t num_programs; // Number of available programs.
	int32_t num_params; // Number of parameters. All programs must have at least this many parameters.
	int32_t num_inputs; // Number of inputs.
	int32_t num_outputs; // Number of outputs.

	/** Effect Flags
	 *
	 * See: VST_EFFECT_FLAGS
	 */
	int32_t flags;

	// 64-bit adds 4-byte padding here to align pointers.

	void* _unknown_00; // Must be zero. Reserved for host?
	void* _unknown_01; // Must be zero. Reserved for host?

	/** Initial delay before processing of samples can actually begin in Samples.
	 *
	 * Note: The host can modify this at runtime so it is not safe. 
	 * Note: Should be reinitialized when the effect is resumed.
	 */
	int32_t delay;

	int32_t _unknown_02; // Unknown int32_t values.
	int32_t _unknown_03;

	/** Ratio of Input to Output production
	 * Defines how much output data is produced relative to input data when using 'process' instead of 'processFloat'.
	 * Example: A ratio of 2.0 means we produce twice as much output as we receive input.
	 * 
	 * Range: >0.0 to Infinity
	 * Default: 1.0
	 * Note: Ignored in VST 2.4 or with VST_EFFECT_FLAG_SUPPORTS_FLOAT.
	 */
	float input_output_ratio;

	/** Effect Internal Pointer
	 * 
	 * You can freely set this to point at some sort of class or similar for use in your own effect. The host must
	 * never modify this or the data available through this.
	 */
	void* effect_internal;

	/** Host Internal Pointer
	 * 
	 * The host may set this to point at data related to your effect instance that the host needs. The effect must
	 * never modify this or the data available through this.
	 */
	void* host_internal; // Pointer to Host internal data.

	/** Id of the plugin.
	 *
	 * Due to this not being enough for uniqueness, it should not be used alone for indexing.
	 * Ideally you want to index like this:
	 *     [unique_id][module_name][version][flags]
	 * If any of the checks after unique_id fail, you default to the first possible choice.
	 * 
	 * Used in combination with VST_EFFECT_CATEGORY_CONTAINER.
	 *
	 * BUG: Some broken hosts rely on this alone to save information about VST plug-ins.
	 */
	int32_t unique_id;

	/** Plugin version
	 * 
	 * Unrelated to the minimum VST Version, but often the same.
	 */
	int32_t version;

	// There is no padding here if everything went right.

	//--------------------------------------------------------------------------------
	// VST 2.x starts here.
	//--------------------------------------------------------------------------------

	/** Process the given number of single samples in inputs and outputs.
	 *
	 * Process input and overwrite the output in place. Host provides output buffers.
	 * 
	 * Note: Not thread-safe on MacOS for some reason or another.
	 * 
	 * @param pthis Pointer to the effect itself.
	 * @param inputs Pointer to an array of 'const float[samples]' with size numInputs.
	 * @param outputs Pointer to an array of 'float[samples]' with size numOutputs.
	 * @param samples Number of samples per channel in inputs.
	 */
	void(VST_FUNCTION_INTERFACE* process_float)(struct vst_effect_t* pthis, const float* const* inputs, float** outputs, int32_t samples);

	//--------------------------------------------------------------------------------
	// VST 2.4 starts here.
	//--------------------------------------------------------------------------------

	/** Process the given number of double samples in inputs and outputs.
	 *
	 * Process input and overwrite the output in place. Host provides output buffers.
	 *
	 * Note: Not present and not called prior to VST 2.4. 
	 * 
	 * @param pthis Pointer to the effect itself.
	 * @param inputs Pointer to an array of 'const double[samples]' with size numInputs.
	 * @param outputs Pointer to an array of 'double[samples]' with size numOutputs.
	 * @param samples Number of samples per channel in inputs.
	 */
	void(VST_FUNCTION_INTERFACE* process_double)(struct vst_effect_t* pthis, const double* const* inputs, double** outputs, int32_t samples);

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
	vst_effect_t* VSTPluginMain(vst_host_callback_t callback)

/** [DEPRECATED] VST 1.x Entry Point for Windows
 *
 * Do not implement in VST 2.1 or later plug-ins!
 * 
 * @return A new instance of the VST 1.x effect.
 */
#define VST_ENTRYPOINT_WINDOWS \
	vst_effect_t* MAIN(vst_host_callback_t callback) { return VSTPluginMain(callback); }

/** [DEPRECATED] VST 1.x Entry Point for MacOS
 *
 * Do not implement in VST 2.1 or later plug-ins!
 * 
 * @return A new instance of the VST 1.x effect.
 */
#define VST_ENTRYPOINT_MACOS \
	vst_effect_t* main_macho(vst_host_callback_t callback) { return VSTPluginMain(callback); }

#ifdef __cplusplus
}
#endif
#pragma pack(pop)
#endif
