#pragma once
#include <wx/wx.h>
#include <wx/dateevt.h>




class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);
private:
	void OnClose(wxCloseEvent& evt);
	void OnClobberChecked(wxCommandEvent& evt);
	void OnCalculateClicked(wxCommandEvent& evt);
	void OnSubmitClicked(wxCommandEvent& evt);
	void OnPrintClicked(wxCommandEvent& evt);
	void OnDateChanged(wxDateEvent& evt);
	void CheckDateExists();
	void CreatePDF();
	wxDECLARE_EVENT_TABLE();
};

