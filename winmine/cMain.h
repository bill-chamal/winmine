#pragma once
#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
public:
	int nFieldWidth =9;
	int nFieldHeight =9;
	wxButton** btn1;
	
	wxMenuBar* Menubar;
	wxMenu* MenuGame;
	wxMenu* MenuAbout;

	int* nField = nullptr;
	bool bFirstClick = true;

	void OnButtonClicked(wxCommandEvent& evt);
	/*Call this wxDECLARE_EVENT_TABLE(); only one time! */
	wxDECLARE_EVENT_TABLE();

private:
	/* Menu Bar*/
	void OnRestart(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnGstat(wxCommandEvent& event);
};
enum
{
	ID_RESTART = 1,
	ID_GSTAT = 2
};
