// synthv1_dpfui.cpp
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

#include "synthv1_dpfui.h"
#include "synthv1_dpf.h"

#include <QWindow>
#include <QApplication>
#include <QMainWindow>

//-------------------------------------------------------------------------
// SynthV1PluginUI - DPF Plugin UI interface.
//

START_NAMESPACE_DISTRHO

SynthV1PluginUI::SynthV1PluginUI()
	: UI(), fWidget(nullptr)
{
	// Print out DPF standalone mode state.
	const bool standalone = isStandalone();
	d_stdout("isStandalone %d", (int)standalone);

	// synthv1 UI requires directly accessing synth instance.
	// This is discouraged by DPF, but synthv1 do need this due to its designation.
	SynthV1Plugin *fDspInstance = (SynthV1Plugin*) UI::getPluginInstancePointer();

	// Load background window.
	// Showing UI within background window can prevent unexpected "ghost shadows" when resizing.
	fWindow = std::make_unique<QMainWindow>();

	// Load synthv1 main UI part.
	fWidget = std::make_unique<synthv1widget_dpf>(fDspInstance->getSynthesizer(), this);

	// Set minimum size
	m_widgetSize = fWidget->sizeHint();
	m_widgetSize.setHeight(m_widgetSize.height() * getScaleFactor());
	m_widgetSize.setWidth(m_widgetSize.width() * getScaleFactor());
	fWidget->setMinimumSize(m_widgetSize);
	fWindow->setMinimumSize(m_widgetSize);

	// Embed synthv1 UI part into background window
	fWidget->setParent(&*fWindow);

	// Explicitly set window position to avoid misplace on some platforms (especially Windows)
	fWidget->move(0, 0);

	// Embed plug-in UI into host window.
	fParent = (WId) getParentWindowHandle();
	fWinId = fWindow->winId();	// Must require WinID first, otherwise plug-in will crash!
	if (fParent)
	{
		fWindow->windowHandle()->setParent(QWindow::fromWinId(fParent));
	}

	// Explicitly show UI. This is required when using external UI mode of DPF.
	fWindow->show();
}

SynthV1PluginUI::~SynthV1PluginUI()
{
	// NOTICE: Now fWidget is already managed by unique pointer. No need to clean up manually.
}

/* --------------------------------------------------------------------------------------------------------
* DSP/Plugin Callbacks */

/**
	A parameter has changed on the plugin side.
	This is called by the host to inform the UI about parameter changes.
*/
void SynthV1PluginUI::parameterChanged(uint32_t index, float value)
{
	fWidget->setUIParamValue(synthv1::ParamIndex(index), value);
}

/* --------------------------------------------------------------------------------------------------------
* External Window overrides */

void SynthV1PluginUI::focus()
{
	d_stdout("focus");

	fWidget->setFocus();
}

uintptr_t SynthV1PluginUI::getNativeWindowHandle() const noexcept
{
	return (uintptr_t)fWidget->windowHandle()->winId();
}

void SynthV1PluginUI::sizeChanged(uint width, uint height)
{
	UI::sizeChanged(width, height);

	if (fWindow != nullptr) {
		fWindow->resize(width, height);
		fWidget->resize(width, height);
	}
}

void SynthV1PluginUI::titleChanged(const char* const title)
{
	d_stdout("titleChanged %s", title);

	DISTRHO_SAFE_ASSERT_RETURN(fWidget != 0,);
	fWidget->setWindowTitle(QString(title));
}

void SynthV1PluginUI::transientParentWindowChanged(const uintptr_t winId)
{
	d_stdout("transientParentWindowChanged %lu", winId);

	DISTRHO_SAFE_ASSERT_RETURN(fWidget != 0,);
	// NOTICE: Seems not implemented by Qt
}

void SynthV1PluginUI::visibilityChanged(const bool visible)
{
	d_stdout("visibilityChanged %d", visible);

	DISTRHO_SAFE_ASSERT_RETURN(fWidget != 0,);

	if (visible)
	{
		fWidget->show();
		fWidget->raise();
		fWidget->activateWindow();
	}
	else
		fWidget->hide();
}

void SynthV1PluginUI::uiIdle()
{
	// d_stdout("uiIdle");

	if (fWidget)
	{
		QApplication::processEvents();
		return;
	}
}

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
	return new SynthV1PluginUI();
}

END_NAMESPACE_DISTRHO


//-------------------------------------------------------------------------
// synthv1_dpfui - Synth engine accessor: impl.
//

synthv1_dpfui::synthv1_dpfui(synthv1_dpf *pSynth, DISTRHO::SynthV1PluginUI *pluginUiInterface)
	: synthv1_ui(pSynth, true), m_plugin_ui(pluginUiInterface)
{
}

void synthv1_dpfui::write_function(synthv1::ParamIndex index, float fValue) const
{
	m_plugin_ui->setParameterValue(index, fValue);
}

// end of synthv1_dpfui.cpp
