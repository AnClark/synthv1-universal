// synthv1widget_dpf.h
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

#ifndef __synthv1widget_dpf_h
#define __synthv1widget_dpf_h

#include "synthv1widget.h"

// Forward decls.
class synthv1_dpf;
class synthv1_dpfui;
namespace DISTRHO {
	class SynthV1PluginUI;
}

//-------------------------------------------------------------------------
// synthv1widget_dpf - decl.
//

class synthv1widget_dpf : public synthv1widget
{
public:

	// Constructor.
	synthv1widget_dpf(synthv1_dpf *pSynth, DISTRHO::SynthV1PluginUI *pPluginUiInterface);

	// Destructor.
	~synthv1widget_dpf();

	// Param method. (Host -> UI)
	void setUIParamValue(synthv1::ParamIndex paramIndex, float value);

protected:

	// Synth engine accessor.
	synthv1_ui *ui_instance() const;

	// Param method. (UI -> Host)
	void updateParam(synthv1::ParamIndex index, float fValue) const;

	// Close event handler.
	void closeEvent(QCloseEvent *pCloseEvent);

private:

	// Instance variables.
	synthv1_dpfui *m_pSynthUi;  // synth engine accessor
};

#endif  // __synthv1widget_dpf_h

// end of synthv1widget_dpf.h
