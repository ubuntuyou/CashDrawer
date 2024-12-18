#include "ChildFrame.h"
#include <wx/wx.h>

ChildFrame::ChildFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	this->SetMinClientSize(wxSize(400, 300));
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* mainPanel = new wxPanel(this);
	mainPanel->SetSizer(mainSizer);
	mainSizer->Fit(this);
}
