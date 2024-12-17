#pragma once
#include <wx/wx.h>
#include <wx/dateevt.h>
#include <wx/vector.h>



class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);
private:
	void OnCalculateClicked(wxCommandEvent& evt);
	void OnSubmitClicked(wxCommandEvent& evt);
	void OnDateChanged(wxDateEvent& evt);
	void CheckDateExists();
	wxDECLARE_EVENT_TABLE();
};

