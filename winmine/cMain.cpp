    // winmine game 
    // Copyright (C) <year>  Bill Chamal

    // This program is free software: you can redistribute it and/or modify
    // it under the terms of the GNU General Public License as published by
    // the Free Software Foundation, either version 3 of the License, or
    // any later version.

    // This program is distributed in the hope that it will be useful,
    // but WITHOUT ANY WARRANTY; without even the implied warranty of
    // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    // GNU General Public License for more details.

    // You should have received a copy of the GNU General Public License
    // along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "cMain.h"
#include <sstream>
#include "resource.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(1, OnButtonClicked)
	EVT_MENU(ID_RESTART, cMain::OnRestart)
	EVT_MENU(wxID_EXIT, cMain::OnExit)
	EVT_MENU(wxID_ABOUT, cMain::OnAbout)
	EVT_MENU(ID_GSTAT, cMain::OnGstat)
wxEND_EVENT_TABLE()

/*Add how many mines and buttons */	
int mines_stc = 10, nFieldHeight=9, nFieldWidth=9;										//*78 line refrerce to mines again*//
int tries = 0, times_clicked = 0, times_restared = 0, times_wins = 0, mines = mines_stc, Win80 = 0, trieys_left = (nFieldHeight* nFieldWidth) - mines;
float score;
cMain::cMain() : wxFrame(nullptr, wxID_ANY, "winmine - 1.1.6 ", wxPoint(30, 30), wxSize(600, 600) )
{
	// Menu Bar
	Menubar = new wxMenuBar();
	// File Menu
	MenuGame = new wxMenu();
	MenuGame->Append(ID_RESTART, _T("&Restart\tCtrl+R"));
	MenuGame->Append(ID_GSTAT, _T("&Status\tCtrl+T"));
	MenuGame->AppendSeparator();
	MenuGame->Append(wxID_EXIT, _T("&Quit\tAlt+f4"));
	// Name Of Bar
	Menubar->Append(MenuGame, _T("&Game"));
	// About menu
	MenuAbout = new wxMenu();
	MenuAbout->Append(wxID_ABOUT, _T("&About"));
	// Name Of Bar
	Menubar->Append(MenuAbout, _T("&Help"));

	SetMenuBar(Menubar);

	// Setting up the ICO for the app named "winmine.ico"
	wxIcon mainicon;
	mainicon.LoadFile("winmine.ico", wxBITMAP_TYPE_ICO);
	SetIcon(mainicon);
	// Adding buttons to gridline
	btn1 = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	nField = new int[nFieldWidth * nFieldHeight];

	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	// Create btns
	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			btn1[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
			btn1[y * nFieldWidth + x]->SetFont(font);
			grid->Add(btn1[y * nFieldWidth + x], 1, wxEXPAND | wxALL);

			// Create commands
			btn1[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			//// Right click ==============           =============
			//btn1[y * nFieldWidth + x]->Bind(wxEVT_RIGHT_DOWN, &cMain::OnButtonRClicked, this);
			// 
			nField[y * nFieldWidth + x] = 0;
		}
	}
	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain()
{
	delete[]btn1;
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	times_clicked++;
	Win80++;
	trieys_left--;
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) /	nFieldWidth;

	if (bFirstClick)
	{
		// there is a secont reference to mine at the top!
		int mines = mines_stc;

		while (mines)
		{
			int rx = rand() % nFieldWidth;
			int ry = rand() % nFieldHeight;

			if (nField[ry * nFieldWidth + rx] == 0 && rx != x && ry != y)
			{
				nField[ry * nFieldWidth + rx] = -1;
				mines--;
			}

		}
		bFirstClick = false;
	}

	btn1[y * nFieldWidth + x]->Enable(false);

	//if(EVT_COMMAND_RIGHT_CLICK)

	if (nField[y * nFieldWidth + x] == -1)
	{
		btn1[y * nFieldWidth + x]->SetLabel("M");
		//btn1[y * nFieldWidth + x]->SetForegroundColour(*wxRED);

		tries++;
		//std::ostringstream oss;
		//oss << tries;
		std::ostringstream trlft, oscore;
		trlft << trieys_left;
		int user_action = wxMessageBox("BOOOOOOOOOOOOOOOOOOOOOM!!!!!\n   - Game Over - "+ trlft.str()+":Clicks Left\nDo you want to start from the beginning?",
			"winmine", wxICON_NONE | wxYES_NO);
		if (user_action == wxYES)
		{
			// reset game
			bFirstClick = true;
			for (int x = 0; x < nFieldWidth; x++)
				for (int y = 0; y < nFieldHeight; y++)
				{
					nField[y * nFieldWidth + x] = 0;
					btn1[y * nFieldWidth + x]->SetLabel("");
					btn1[y * nFieldWidth + x]->Enable(true);
					//btn1[y * nFieldWidth + x]->SetForegroundColour(*wxBLACK);
				}
			Win80 = 0;
			trieys_left = (nFieldHeight * nFieldWidth) - mines;
		}
		else if(user_action == wxNO)
		{
			wxExit();
		}
	}
	else
	{
		// cout naighbouring mines
		int mine_cout = 0;
		for(int i = -1; i<2; i++)
			for (int j = -1; j < 2; j++)
			{
				if (x + i >= 0 && x + i < nFieldWidth && y + i >= 0 && y + j < nFieldHeight)
				{
					if (nField[(y + j) * nFieldWidth + (x + i)] == -1)
						mine_cout++;
				}
			}
		// update btn lbl to show mine cout if>0
		if (mine_cout > 0)
			btn1[y * nFieldWidth + x]->SetLabel(std::to_string(mine_cout));
	}
	// Check if player Wins!
	if (Win80 == (nFieldHeight * nFieldWidth) - mines)
	{
		times_wins++;
		std::ostringstream ossw;
		ossw << times_wins;
		std::ostringstream oss;
		oss << tries;
		int user_action = wxMessageBox(" ---- You Win!!! ---- \nTimes Wins:"+ossw.str()+" Times Losed:"+oss.str()+"\nDo you want to start from the beginning? ",
			"winmine", wxYES_NO | wxICON_INFORMATION );
		if (user_action == wxYES)
		{
			// reset game
			bFirstClick = true;
			for (int x = 0; x < nFieldWidth; x++)
				for (int y = 0; y < nFieldHeight; y++)
				{
					nField[y * nFieldWidth + x] = 0;
					btn1[y * nFieldWidth + x]->SetLabel("");
					btn1[y * nFieldWidth + x]->Enable(true);
				}
			Win80 = 0;
			trieys_left = (nFieldHeight * nFieldWidth) - mines;
		}
		else if (user_action == wxNO)
		{
			wxExit();
		}

	}
	evt.Skip();
}

void cMain::OnRestart(wxCommandEvent& event)
{
	times_restared++;
	// reset game
	bFirstClick = true;
	for (int x = 0; x < nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
		{
			nField[y * nFieldWidth + x] = 0;
			btn1[y * nFieldWidth + x]->SetLabel("");
			btn1[y * nFieldWidth + x]->Enable(true);
		}
	Win80 = 0;
	trieys_left = (nFieldHeight * nFieldWidth) - mines;
}

void cMain::OnExit(wxCommandEvent& event)
{
	wxExit();
}

void cMain::OnAbout(wxCommandEvent& event)
{
	std::ostringstream onFieldHeight, onFieldWidth;
	onFieldHeight << nFieldHeight;
	onFieldWidth << nFieldWidth;
	wxMessageBox("winmine "+ onFieldWidth .str()+"x"+onFieldHeight.str()+"\nVersion 2019 (OS Build 1.1.6)\nAuthor Bill Chamalidis\n2019 BC All Rights Reserved.\n\nwxWidgets version 3.1.3\nC++ 17\nVisual Studio 2019 (v142)\n\nThis program is free software: you can redistribute it and/or modify it\nunder the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or any \nlater version.",
		"winmine Info", wxOK);
}

void cMain::OnGstat(wxCommandEvent& event)
{
	std::ostringstream osst;
	osst << times_clicked;
	std::ostringstream oss;
	oss << tries;
	std::ostringstream ossr;
	ossr << times_restared;
	std::ostringstream ossw;
	ossw << times_wins;
	std::ostringstream osstt, ossmines;
	osstt << tries + times_wins;
	ossmines << mines;
	std::ostringstream trlft, oscore;
	trlft << trieys_left;
	oscore << times_clicked*0.45 + times_wins*4;
	wxMessageBox("Game Status:\n-Score:"+oscore.str()+"\n-Mines:"+ossmines.str()+"\n-Wins:"+ossw.str()+"\n-Times Clicked:"+osst.str()+"\n-Times Losed:"+oss.str()+"\n-Times Restarted:"+ossr.str()+"\n-Times Played:"+osstt.str()+"\n-Clicks Left:"+ trlft.str(),
		"winmine Status", wxOK | wxICON_INFORMATION);
}
