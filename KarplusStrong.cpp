#include "KarplusStrong.h"

KarplusStrong::KarplusStrong(Inputs* inputs)
{
	
	ModuleEqDrum *drum_sound = new ModuleEqDrum();
	ModuleLowpassFilter *lowpass_filter = new ModuleLowpassFilter();
	ModuleENV *envelope_generator = new ModuleENV();
	ModuleVCA *vca = new ModuleVCA();
	ModuleMixer2 *mixer = new ModuleMixer2();
	ModuleMixer2 *mixer2 = new ModuleMixer2();
	ModuleDelay *delay = new ModuleDelay();
	ModuleMap *map = new ModuleMap(750,20);
	ModuleQuantizer *quantizer = new ModuleQuantizer();
	
	// Set up map
	map->input = inputs->param3;

	// Patch up envelope generator
	envelope_generator->slope_input = new ModuleConstant(5);
	envelope_generator->trigger_input = inputs->gate;
	envelope_generator->frequency_input = new ModuleConstant(10);
	
	// Patch up drum - also works with Wavetable
	drum_sound->sample_rate_input    = inputs->sr;
	drum_sound->drum_selection_input = inputs->mod;
	drum_sound->trigger_input        = inputs->gate;
	
	// Patch up VCA
	vca->cv_input = envelope_generator;
	vca->audio_input = drum_sound;

	// Patch up delay
	delay->audio_input = mixer;
	delay->mix_input = new ModuleConstant(4095);
	delay->feedback_input = new ModuleConstant(3900);
	delay->length_input = map;

	//Quantize it just because
	quantizer->cv_input = delay;
	quantizer->scale_input = new ModuleConstant(0);
	
	// Patch up filter
	lowpass_filter->audio_input = quantizer;
	lowpass_filter->cutoff_input = inputs->param1;
	lowpass_filter->resonance_input = inputs->param2;

	// Patch up mixer
	mixer->input_1 = vca;
	mixer->input_2 = lowpass_filter;
	
	// Patch redundant mixer
	mixer2->input_1 = new ModuleConstant(0);
	mixer2->input_2 = lowpass_filter;

	this->last_module = mixer2;

}
