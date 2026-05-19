#pragma once

#include "vst.h"
#include <cstring>

// using strcpy causes warning
#pragma warning(disable : 4996)

class vstbase
{
	const vst_host_callback_t host;

protected:
	vst_effect_t effect;

    const char* product;
    const char* vendor;
    const char* name;
    intptr_t version;
    intptr_t category;

public:
	vstbase(vst_host_callback_t callback):host(callback)
	{
        memset(&effect, 0, sizeof(effect));

        effect.magic_number = VST_MAGICNUMBER;
        effect.unique_id = VST_FOURCC('T','E','S','T');
        effect.version = VST_VERSION_2_4_0_0;
        effect.num_params = 0;
        effect.num_programs = 0;
        effect.num_inputs = 0;
        effect.num_outputs = 0;
        effect.input_output_ratio = 1;
        
        // plugin flags
        effect.flags |= VST_EFFECT_FLAG_SUPPORTS_FLOAT;
        effect.flags |= VST_EFFECT_FLAG_EDITOR;
        effect.flags |= VST_EFFECT_FLAG_CHUNKS;
        //effect.flags |= VST_EFFECT_FLAG_SUPPORTS_DOUBLE;

        // static calls
        effect.control = &vst_control;
        effect.process = &vst_process;
        effect.get_parameter = &vst_getParameter;
        effect.set_parameter = &vst_setParameter;
        effect.process_float = &vst_processFloat;
        effect.process_double = &vst_processDouble;
        
        // other
        product = "Test";
        vendor = "TestingCompany";
        name = "TestVst";
        category = VST_EFFECT_CATEGORY_METERING;
        version = 0;

        effect.effect_internal = this;  // important for the callbacks
	}

    vst_effect_t* getEffect()
    {
        return &effect;
    }

private:
    //____________________________________________________________
    //                     Effect Callbacks
    //____________________________________________________________

    static intptr_t VST_FUNCTION_INTERFACE vst_control(struct vst_effect_t* self, int32_t opcode, int32_t p_int1, intptr_t p_int2, void* p_ptr, float p_float)
    {
        return ((vstbase*)(self->effect_internal))->control(opcode, p_int1, p_int2, p_ptr, p_float);
    }

    static void VST_FUNCTION_INTERFACE vst_process(struct vst_effect_t* self, const float* const* inputs, float** outputs, int32_t samples)
    {
        //obsolete
    }

    static void VST_FUNCTION_INTERFACE vst_processFloat(struct vst_effect_t* self, const float* const* inputs, float** outputs, int32_t samples)
    {
        ((vstbase*)(self->effect_internal))->processReplacing(inputs, outputs, samples);
    }

    static void VST_FUNCTION_INTERFACE vst_processDouble(struct vst_effect_t* self, const double* const* inputs, double** outputs, int32_t samples)
    {
        ((vstbase*)(self->effect_internal))->processReplacingDouble(inputs, outputs, samples);
    }

    static float VST_FUNCTION_INTERFACE vst_getParameter(struct vst_effect_t* self, uint32_t index)
    {
        return ((vstbase*)(self->effect_internal))->getParameter(index);
    }

    static void VST_FUNCTION_INTERFACE vst_setParameter(struct vst_effect_t* self, uint32_t index, float value)
    {
        ((vstbase*)(self->effect_internal))->setParameter(index, value);
    }

    intptr_t VST_FUNCTION_INTERFACE control(int32_t opcode, int32_t p_int1, intptr_t p_int2, void* p_ptr, float p_float)
    {
        intptr_t ret = 0;

        switch (opcode)
        {
        case VST_EFFECT_OPCODE_INITIALIZE:
            open();
            break;
        case VST_EFFECT_OPCODE_DESTROY:
            close();
            break;
        case VST_EFFECT_OPCODE_SET_PROGRAM:
        case VST_EFFECT_OPCODE_GET_PROGRAM:
        case VST_EFFECT_OPCODE_SET_PROGRAM_NAME:
        case VST_EFFECT_OPCODE_GET_PROGRAM_NAME:
        case VST_EFFECT_OPCODE_PARAM_GETLABEL:
        case VST_EFFECT_OPCODE_PARAM_GETVALUE:
        case VST_EFFECT_OPCODE_PARAM_GETNAME:
        case VST_EFFECT_OPCODE_09:
            break;

        case VST_EFFECT_OPCODE_SETSAMPLERATE:
            setSampleRate(p_float);
            break;
        case VST_EFFECT_OPCODE_SETBLOCKSIZE:
            break;
        case VST_EFFECT_OPCODE_SUSPEND_RESUME:
            if (!p_int2) suspend(); else resume();
            break;
        case VST_EFFECT_OPCODE_EDITOR_GET_RECT:
            ret = editorRect((vst_rect_t**)p_ptr);
            break;
        case VST_EFFECT_OPCODE_EDITOR_OPEN:
            ret = editorOpen(p_ptr);
            break;
        case VST_EFFECT_OPCODE_EDITOR_CLOSE:
            editorClose();
            break;

        case VST_EFFECT_OPCODE_EDITOR_DRAW:
        case VST_EFFECT_OPCODE_EDITOR_MOUSE:
        case VST_EFFECT_OPCODE_EDITOR_KEYBOARD:
        case VST_EFFECT_OPCODE_EDITOR_KEEP_ALIVE:
        case VST_EFFECT_OPCODE_14:
        case VST_EFFECT_OPCODE_15:
        case VST_EFFECT_OPCODE_FOURCC:
        case VST_EFFECT_OPCODE_GET_CHUNK_DATA:
        case VST_EFFECT_OPCODE_SET_CHUNK_DATA:
        case VST_EFFECT_OPCODE_EVENT:
        case VST_EFFECT_OPCODE_PARAM_AUTOMATABLE:
        case VST_EFFECT_OPCODE_PARAM_SET_VALUE:
        case VST_EFFECT_OPCODE_1C:
        case VST_EFFECT_OPCODE_1D:
        case VST_EFFECT_OPCODE_1E:
        case VST_EFFECT_OPCODE_1F:
        case VST_EFFECT_OPCODE_20:
        case VST_EFFECT_OPCODE_INPUT_GET_PROPERTIES:
        case VST_EFFECT_OPCODE_OUTPUT_GET_PROPERTIES:
            break;

        case VST_EFFECT_OPCODE_EFFECT_CATEGORY:
            return category;

        case VST_EFFECT_OPCODE_24:
        case VST_EFFECT_OPCODE_25:
        case VST_EFFECT_OPCODE_26:
        case VST_EFFECT_OPCODE_27:
        case VST_EFFECT_OPCODE_28:
        case VST_EFFECT_OPCODE_29:
        case VST_EFFECT_OPCODE_SET_SPEAKER_ARRANGEMENT:
        case VST_EFFECT_OPCODE_2B:
        case VST_EFFECT_OPCODE_BYPASS:
            break;
        case VST_EFFECT_OPCODE_EFFECT_NAME:
            std::strcpy((char*)p_ptr, name);
            return true;
        case VST_EFFECT_OPCODE_TRANSLATE_ERROR:
            break;
        case VST_EFFECT_OPCODE_VENDOR_NAME:
            std::strcpy((char*)p_ptr, vendor);
            return true;
        case VST_EFFECT_OPCODE_PRODUCT_NAME:
            std::strcpy((char*)p_ptr, product);
            return true;
        case VST_EFFECT_OPCODE_VENDOR_VERSION:
            return version;

        case VST_EFFECT_OPCODE_CUSTOM:
        case VST_EFFECT_OPCODE_SUPPORTS:
        case VST_EFFECT_OPCODE_GETTAILSAMPLES:
        case VST_EFFECT_OPCODE_IDLE:
        case VST_EFFECT_OPCODE_36:
        case VST_EFFECT_OPCODE_37:
        case VST_EFFECT_OPCODE_PARAM_PROPERTIES:
        case VST_EFFECT_OPCODE_39:
            break;

        case VST_EFFECT_OPCODE_VST_VERSION:
            return VST_VERSION_2_4_0_0;

        case VST_EFFECT_OPCODE_EDITOR_VKEY_DOWN:
        case VST_EFFECT_OPCODE_EDITOR_VKEY_UP:
        case VST_EFFECT_OPCODE_3D:
        case VST_EFFECT_OPCODE_3E:
        case VST_EFFECT_OPCODE_3F:
        case VST_EFFECT_OPCODE_40:
        case VST_EFFECT_OPCODE_41:
        case VST_EFFECT_OPCODE_42:
        case VST_EFFECT_OPCODE_PROGRAM_SET_BEGIN:
        case VST_EFFECT_OPCODE_PROGRAM_SET_END:
            break;

        case VST_EFFECT_OPCODE_GET_SPEAKER_ARRANGEMENT:
            ret = getSpeakerArrangement((vst_speaker_arrangement_t**)p_int2, (vst_speaker_arrangement_t**)p_ptr);
            break;

        case VST_EFFECT_OPCODE_CONTAINER_NEXT_EFFECT_ID:
        case VST_EFFECT_OPCODE_PROCESS_BEGIN:
        case VST_EFFECT_OPCODE_PROCESS_END:
        case VST_EFFECT_OPCODE_49:
        case VST_EFFECT_OPCODE_4A:
        case VST_EFFECT_OPCODE_BANK_LOAD:
        case VST_EFFECT_OPCODE_PROGRAM_LOAD:
        case VST_EFFECT_OPCODE_4D:
        case VST_EFFECT_OPCODE_4E:
        case VST_EFFECT_OPCODE_4F:
            break;
        default:
            break;
        }


        return ret;
    }
    
    //____________________________________________________________
    //                     VstBase Callbacks
    //____________________________________________________________
protected:

    virtual void processReplacing(const float* const* inputs, float** outputs, int32_t samples) {};
    virtual void processReplacingDouble(const double* const* inputs, double** outputs, int32_t samples) {};
    virtual float getParameter(uint32_t index) { return 0; }
    virtual void setParameter(uint32_t index, float value) {}
    virtual void setSampleRate(float sampleRate) = 0;
    virtual bool getSpeakerArrangement(vst_speaker_arrangement_t** pluginInput, vst_speaker_arrangement_t** pluginOutput) = 0;

    virtual void open() = 0;
    virtual void close() = 0;
    virtual void suspend() = 0;
    virtual void resume() = 0;

    // editor state
    virtual bool editorOpen(void* parent) = 0;
    virtual void editorClose() = 0;
    virtual bool editorRect(vst_rect_t** rect) = 0;

};