#pragma once

#include "..\include\vstbase.h"

using namespace std;

class EmptyVst : public vstbase
{
	static const int ChannelsCount = 2;

	vst_speaker_arrangement_t speakers{0};
	vst_rect_t rect{ 0 };

public:
    
	EmptyVst(vst_host_callback_t callback);
	~EmptyVst();

	// Processing
	virtual void processReplacing(const float* const* inputs, float** outputs, int32_t sampleFrames) override;
	virtual void setSampleRate(float sampleRate) override;
	virtual bool getSpeakerArrangement(vst_speaker_arrangement_t** pluginInput, vst_speaker_arrangement_t** pluginOutput) override;

	// vst state
	virtual void open() override;
	virtual void close() override;
	virtual void suspend() override;
	virtual void resume() override;

	// editor state
	virtual bool editorOpen(void* parent) override;
	virtual void editorClose() override;
	virtual bool editorRect(vst_rect_t** rect) override;
};