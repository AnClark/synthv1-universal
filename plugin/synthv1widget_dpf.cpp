// synthv1widget_dpf.cpp
//
/****************************************************************************
   Copyright (C) 2012-2022, rncbc aka Rui Nuno Capela.
   Copyright (C) 2023, AnClark Liu.
   All rights reserved.

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

#include "synthv1widget_dpf.h"

#include "synthv1_dpf.h"
#include "synthv1_dpfui.h"

#include <QApplication>
#include <QFileInfo>
#include <QDir>

#include "synthv1widget_palette.h"

#include <QCloseEvent>

#include <QStyleFactory>

#ifndef CONFIG_LIBDIR
#if defined(__x86_64__)
#define CONFIG_LIBDIR CONFIG_PREFIX "/lib64"
#else
#define CONFIG_LIBDIR CONFIG_PREFIX "/lib"
#endif
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define CONFIG_PLUGINSDIR CONFIG_LIBDIR "/qt5/plugins"
#else
#define CONFIG_PLUGINSDIR CONFIG_LIBDIR "/qt6/plugins"
#endif


//-------------------------------------------------------------------------
// synthv1widget_lv2 - impl.
//

synthv1widget_dpf::synthv1widget_dpf ( synthv1_dpf *pSynth, DISTRHO::SynthV1PluginUI *pPluginUiInterface )
	: synthv1widget()
{
	// Check whether under a dedicated application instance...
	QApplication *pApp = synthv1_dpf::qapp_instance();
	if (pApp) {
		// Special style paths...
		if (QDir(CONFIG_PLUGINSDIR).exists())
			pApp->addLibraryPath(CONFIG_PLUGINSDIR);
	}

	// Custom color/style themes...
	synthv1_config *pConfig = synthv1_config::getInstance();
	if (pConfig) {
		if (!pConfig->sCustomColorTheme.isEmpty()) {
			QPalette pal;
			if (synthv1widget_palette::namedPalette(
					pConfig, pConfig->sCustomColorTheme, pal))
				synthv1widget::setPalette(pal);
		}
		if (!pConfig->sCustomStyleTheme.isEmpty()) {
			synthv1widget::setStyle(
				QStyleFactory::create(pConfig->sCustomStyleTheme));
		}
	}

	// Initialize (user) interface stuff...
	m_pSynthUi = new synthv1_dpfui(pSynth, pPluginUiInterface);

	// Initialise preset stuff...
	clearPreset();

	// Initial update, always...
	//resetParamValues();
	resetParamKnobs();

	// May initialize the scheduler/work notifier.
	openSchedNotifier();
}


// Destructor.
synthv1widget_dpf::~synthv1widget_dpf (void)
{
	delete m_pSynthUi;
}


// Synth engine accessor.
synthv1_ui *synthv1widget_dpf::ui_instance (void) const
{
	return m_pSynthUi;
}

// Close event handler.
void synthv1widget_dpf::closeEvent ( QCloseEvent *pCloseEvent )
{
	synthv1widget::closeEvent(pCloseEvent);
}

// Param method: Host -> UI.
// Render host's parameter values on UI. This is called by DPF UI's paramChanged() method.
void synthv1widget_dpf::setUIParamValue(synthv1::ParamIndex paramIndex, float value)
{
	this->setParamValue(paramIndex, value);
}

// Param method: UI -> Host.
// This method sets host's param values from UI side.
void synthv1widget_dpf::updateParam (
	synthv1::ParamIndex index, float fValue ) const
{
	m_pSynthUi->setParamValue(index, fValue);
	m_pSynthUi->write_function(index, fValue);
}

// end of synthv1widget_dpf.cpp
