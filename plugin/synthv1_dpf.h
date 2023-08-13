// synthv1_dpf.h
//
/****************************************************************************
   Copyright (C) 2023, AnClark Liu. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#ifndef __synthv1_dpf_h
#define __synthv1_dpf_h

#include "synthv1.h"
#include "config_dpf.h"

#include "DistrhoPlugin.hpp"

#include <memory>

// Forward decls.
class QApplication;


//-------------------------------------------------------------------------
// synthv1_dpf - Synthesizer interface decl.
//

class synthv1_dpf : public synthv1
{
public:

	synthv1_dpf(double sample_rate);

	~synthv1_dpf();

	void run(const float **inputs, float **outputs, uint32_t nframes, const MidiEvent* midiEvents, uint32_t midiEventCount);

	void activate();
	void deactivate();

	uint32_t urid_map(const char *uri) const;

	static void qapp_instantiate();
	static void qapp_cleanup();

	static QApplication *qapp_instance();

protected:

	void updatePreset(bool bDirty);
	void updateParam(synthv1::ParamIndex index);
	void updateParams();
	void updateTuning();

	bool state_changed();

private:

	static QApplication *g_qapp_instance;
	static unsigned int  g_qapp_refcount;
};


//-------------------------------------------------------------------------
// SynthV1Plugin - The main DPF plugin interface decl.
//

START_NAMESPACE_DISTRHO

class SynthV1Plugin : public Plugin
{
	double fSampleRate = getSampleRate();
	std::unique_ptr<synthv1_dpf> fSynthesizer = std::make_unique<synthv1_dpf>(fSampleRate);

public:
	SynthV1Plugin();
	~SynthV1Plugin();

	synthv1_dpf *getSynthesizer();

protected:
	// ----------------------------------------------------------------------------------------------------------------
	// Information

   /**
	  Get the plugin label.@n
	  This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
	*/
	const char* getLabel() const noexcept override
	{
		return "synthv1";
	}

   /**
	  Get an extensive comment/description about the plugin.@n
	  Optional, returns nothing by default.
	*/
	const char* getDescription() const override
	{
		return "An old-school subtractive polyphonic synthesizer with stereo fx";
	}

   /**
	  Get the plugin author/maker.
	*/
	const char* getMaker() const noexcept override
	{
		return "rncbc aka. Rui Nuno Capela";
	}

   /**
	  Get the plugin license (a single line of text or a URL).@n
	  For commercial plugins this should return some short copyright information.
	*/
	const char* getLicense() const noexcept override
	{
		return "GPLv2";
	}

   /**
	  Get the plugin version, in hexadecimal.
	  @see d_version()
	*/
	uint32_t getVersion() const noexcept override
	{
		return d_version(CONFIG_VERSION_MAJOR, CONFIG_VERSION_MINOR, CONFIG_VERSION_PATCH);
	}

   /**
	  Get the plugin unique Id.@n
	  This value is used by LADSPA, DSSI and VST plugin formats.
	  @see d_cconst()
	*/
	int64_t getUniqueId() const noexcept override
	{
		return d_cconst('s', 'y', 'h', '1');
	}

	// ----------------------------------------------------------------------------------------------------------------
	// Init

	void initParameter(uint32_t index, Parameter& parameter) override;

	// ----------------------------------------------------------------------------------------------------------------
	// Internal data

	float getParameterValue(uint32_t index) const override;
	void setParameterValue(uint32_t index, float value) override;

	// ----------------------------------------------------------------------------------------------------------------
	// Audio/MIDI Processing

	void activate() override;
	void run(const float** inputs, float** outputs, uint32_t frames, const MidiEvent* midiEvents, uint32_t midiEventCount) override;

	// ----------------------------------------------------------------------------------------------------------------
	// Callbacks (optional)

	void sampleRateChanged(double newSampleRate) override;

	// ----------------------------------------------------------------------------------------------------------------

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthV1Plugin)
};

END_NAMESPACE_DISTRHO

#endif// __synthv1_dpf_h

// end of synthv1_dpf.h
