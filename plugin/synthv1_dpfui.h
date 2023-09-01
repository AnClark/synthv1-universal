// synthv1_dpfui.h
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

#ifndef __synthv1_dpfui_h
#define __synthv1_dpfui_h

#include "synthv1widget_dpf.h"

#include "DistrhoUI.hpp"

// Forward decls.
class synthv1_dpf;
class QMainWindow;

// -----------------------------------------------------------------------------------------------------------
// SynthV1PluginUI - DPF Plugin UI interface.

START_NAMESPACE_DISTRHO

class SynthV1PluginUI : public UI {

	std::unique_ptr<QMainWindow> fWindow;	// Background window
	std::unique_ptr<synthv1widget_dpf> fWidget;	// The main UI part
	WId fWinId;
	WId fParent;

	QSize m_widgetSize;	// Target Qt UI size with scale factor considered

	// ----------------------------------------------------------------------------------------------------------------

public:
	SynthV1PluginUI();
	~SynthV1PluginUI();

protected:
	// ----------------------------------------------------------------------------------------------------------------
	// DSP/Plugin Callbacks

	void parameterChanged(uint32_t index, float value) override;
	//void programLoaded(uint32_t index) override;
	//void stateChanged(const char* key, const char* value) override;

	// ----------------------------------------------------------------------------------------------------------------
	// External window overrides

	void focus() override;
	uintptr_t getNativeWindowHandle() const noexcept override;
	void sizeChanged(uint width, uint height) override;
	void titleChanged(const char* const title) override;
	void transientParentWindowChanged(const uintptr_t winId) override;
	void visibilityChanged(const bool visible) override;
	void uiIdle() override;

private:
	// ----------------------------------------------------------------------------------------------------------------
	// Internal Procedures

	void _initParameterProperties();

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthV1PluginUI)
};

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO

//-------------------------------------------------------------------------
// synthv1_dpfui - Synth engine accessor: decl.
//

class synthv1_dpfui : public synthv1_ui
{
public:

	// Constructor.
	synthv1_dpfui(synthv1_dpf *pSynth, DISTRHO::SynthV1PluginUI *pluginUiInterface);

	// Accessors.
	void write_function(synthv1::ParamIndex index, float fValue) const;

private:
	DISTRHO::SynthV1PluginUI *m_plugin_ui;
};

#endif// __synthv1_dpfui_h

// end of synthv1_dpfui.h
