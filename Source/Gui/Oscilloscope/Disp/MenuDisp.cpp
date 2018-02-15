#include "MenuDisp.h"

#include <Source/Gui/MainWnd.h>

/*virtual*/ void CWndMenuDisplay::Create(CWnd *pParent, ui16 dwFlags) 
{
	CWnd::Create("CWndMenuDisp", dwFlags, CRect(320-CWndMenuItem::MarginLeft, 20, 400, 240), pParent);

	m_proAxes.Create( (const char**)CSettings::Display::ppszTextAxes,
		(NATIVEENUM*)&Settings.Disp.Axes, CSettings::Display::_AxesMax );
	m_proDraw.Create( (const char**)CSettings::Display::ppszTextDraw,
		(NATIVEENUM*)&Settings.Disp.Draw, CSettings::Display::_DrawMax );
	m_proAverage.Create( (const char**)CSettings::Display::ppszTextAverage,
		(NATIVEENUM*)&Settings.Disp.Average, CSettings::Display::_AverageMax );
	m_proPersist.Create( (const char**)CSettings::Display::ppszTextPersist,
		(NATIVEENUM*)&Settings.Disp.Persist, CSettings::Display::_PersistMax );
	m_proGrid.Create( (const char**)CSettings::Display::ppszTextGrid,
		(NATIVEENUM*)&Settings.Disp.Grid, CSettings::Display::_GridMax );
	m_proAxis.Create( (const char**)CSettings::Display::ppszTextAxis,
		(NATIVEENUM*)&Settings.Disp.Axis, CSettings::Display::_AxisMax );

	m_itmAxes.Create("Mode", RGB565(808080), &m_proAxes, this);
	m_itmDraw.Create("Display", RGB565(808080), &m_proDraw, this);
	m_itmAverage.Create("Average", RGB565(808080), &m_proAverage, this);
	m_itmPersist.Create("Persist", RGB565(808080), &m_proPersist, this);
	m_itmGrid.Create("Grid", RGB565(808080), &m_proGrid, this);
	m_itmAxis.Create("Axis", RGB565(808080), &m_proAxis, this);
}

/*virtual*/ void CWndMenuDisplay::OnMessage(CWnd* pSender, CodeParam code, DataParam data)
{
	// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
	if (code == ToWord('L', 'D') )
	{
		MainWnd.m_wndGraph.ShowWindow( SwHide );
		MainWnd.m_wndInfoBar.ShowWindow( SwHide );
		MainWnd.m_wndLReferences.ShowWindow( SwHide );
		MainWnd.m_wndTReferences.ShowWindow( SwHide );
	}

	if (code == ToWord('L', 'E') )
	{
		MainWnd.m_wndGraph.ShowWindow( SwShow );
		MainWnd.m_wndInfoBar.ShowWindow( SwShow );
		MainWnd.m_wndLReferences.ShowWindow( SwShow );
		MainWnd.m_wndTReferences.ShowWindow( SwShow );
	}

	if ( code == ToWord('l', 'e') && data == (DataParam)&m_proAverage )
	{
		MainWnd.m_wndGraph.ClearAverage();
	}
	if ( code == ToWord('l', 'e') && data == (DataParam)&m_proPersist )
	{	
		MainWnd.m_wndGraph.ClearPersist();
	}
	if ( code == ToWord('u', 'p') && pSender == &m_itmAxes )
	{	
		// force redraw background (invalidate is not enough)
		MainWnd.m_wndGraph.ShowWindow( CWnd::SwHide );
		MainWnd.m_wndGraph.ShowWindow( CWnd::SwShow );
	}

	/* // produces nice effect when not cleared :)
	if ( code == ToWord('u', 'p') && pSender == &m_itmAverage )
	{
		MainWnd.m_wndGraph.ClearAverage();
	}*/
}


