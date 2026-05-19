#include "EmptyVst.h"

vstbase* createPlugin(vst_host_callback_t callback)
{
	return new EmptyVst(callback);
}

EmptyVst::EmptyVst(vst_host_callback_t callback) : vstbase(callback)
{
	effect.num_inputs = ChannelsCount;
	effect.num_outputs = ChannelsCount;
	effect.unique_id = VST_FOURCC('E', 'M', 'P', 'T');
	product = "description about the plugin";
	vendor = "Beklemeto";
	name = "EmptyVst";
	category = VST_EFFECT_CATEGORY_METERING;
	version = 101;
}

void EmptyVst::processReplacing(const float* const* inputs, float** outputs, int32_t samples)
{
	int chanCount = ChannelsCount; 

	for (int i = 0; i < chanCount; i++)
	{
		auto inputSamples = inputs[i];
		auto outputSamples = outputs[i];

		for (int j = 0; j < samples; j++)
		{
			outputSamples[j] = inputSamples[j] * 0.5f;
		}
	}
}

void EmptyVst::setSampleRate(float sampleRate)
{ 
	
}

bool EmptyVst::getSpeakerArrangement(vst_speaker_arrangement_t** pluginInput, vst_speaker_arrangement_t** pluginOutput)
{
	speakers.channels = ChannelsCount;
	speakers.type = VST_SPEAKER_ARRANGEMENT_TYPE_STEREO;

	*pluginInput = &speakers;
	*pluginOutput = &speakers;

	return true;
}

void EmptyVst::open()
{
}

void EmptyVst::resume()
{
}

void EmptyVst::suspend()
{
}

void EmptyVst::close()
{
}

EmptyVst::~EmptyVst()
{
}

bool EmptyVst::editorOpen(void* parent)
{	
	return true;
}

void EmptyVst::editorClose()
{
}

bool EmptyVst::editorRect(vst_rect_t** rect)
{
	*rect = &(this->rect);
		
	this->rect.top = 100;
	this->rect.left = 100;
	this->rect.bottom = 200;
	this->rect.right = 300;
			
	return true;
}