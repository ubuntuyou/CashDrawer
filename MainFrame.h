#pragma once
#include <wx/wx.h>
#include <wx/dateevt.h>



class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);
private:
	void OnCalculateClicked(wxCommandEvent& evt);
	void OnSubmitClicked(wxCommandEvent& evt);
	void OnDateChanged(wxDateEvent& evt);
	wxDECLARE_EVENT_TABLE();
};

