#ifndef __GENERATOR_MENUMAINEDIT_H__
#define __GENERATOR_MENUMAINEDIT_H__

#include <Source/Gui/Generator/Core/CoreGenerator.h>
#include <Source/Core/Settings.h>
#include <Source/Framework/Wnd.h>
#include <Source/Core/ListItems.h>

class CWndMenuGeneratorEdit : public CWnd
{
public:
	CWndMenuItem	m_itmCopy;
	CWndMenuItem	m_itmNormalize;
	CWndMenuItem	m_itmInvert;
	CWndMenuItem	m_itmEdit;
	CWndMenuItem	m_itmSave;
	CWndMenuItem	m_itmLoad;

	virtual void Create(CWnd *pParent, ui16 dwFlags);
	virtual void OnMessage(CWnd* pSender, WPARAM code, LPARAM data);
};

#endif