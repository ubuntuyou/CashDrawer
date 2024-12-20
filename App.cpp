#include "App.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("Cash Drawer");
	ChildFrame* childFrame = new ChildFrame("Child Frame");
	mainFrame->Center();
	mainFrame->Show();
	//childFrame->Show();
	return true;
}