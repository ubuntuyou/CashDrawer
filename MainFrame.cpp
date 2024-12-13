#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/vector.h>

#define DRAWER_LABEL 20
#define DRAWER_TOTAL 21
#define ONHAND_LABEL 22
#define ONHAND_TOTAL 23
#define ACTPAY_LABEL 24
#define ACTPAY_TOTAL 25
#define DIFFER_LABEL 26
#define DIFFER_TOTAL 27

std::string fields[] = { "Pennies", "Nickels", "Dimes", "Quarters", "Ones", "Fives", "Tens", "Twenties", "Fifties", "Hundreds", "Checks", "Bank Acct.", "M.V. Due", "Petty Cash", "Cash Bag", "Office", "Copy", "Firearms", "Prints" };

wxVector<wxString> entry_dates;
wxStaticText* label[28];
wxTextCtrl* entry[19];
wxDouble value[19];
wxDatePickerCtrl* picker;
wxString date;
wxString new_line;
size_t line_count;
size_t line;
wxStringTokenizer tokenizer;
wxTextFile file(wxDateTime::Today().Format("%Y_%m") + ".csv");


enum IDs {
	CALCULATE = 2,
	SUBMIT = 3,
	DPICKER = 4
};


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(CALCULATE, MainFrame::OnCalculateClicked)
EVT_BUTTON(SUBMIT, MainFrame::OnSubmitClicked)
EVT_DATE_CHANGED(DPICKER, MainFrame::OnDateChanged)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	this->SetMinClientSize(wxSize(325, 525));
	this->SetMaxClientSize(wxSize(325, 525));

	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* date_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* entry_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* button_sizer = new wxBoxSizer(wxVERTICAL);
	wxFlexGridSizer* gsizer1 = new wxFlexGridSizer(13, 2, 10, 8);
	wxFlexGridSizer* gsizer2 = new wxFlexGridSizer(13, 2, 10, 8);
	wxFlexGridSizer* bsizer = new wxFlexGridSizer(1, 4, 10, 10);
	wxFlexGridSizer* dsizer = new wxFlexGridSizer(1, 2, 10, 10);

	wxPanel* main_panel = new wxPanel(this);
	wxPanel* date_panel = new wxPanel(main_panel);
	wxPanel* left_panel = new wxPanel(main_panel);
	wxPanel* right_panel = new wxPanel(main_panel);
	wxPanel* button_panel = new wxPanel(main_panel);

/*
	main_panel->SetBackgroundColour(wxColour(100, 100, 100));
	date_panel->SetBackgroundColour(wxColour(150, 150, 150));
	button_panel->SetBackgroundColour(wxColour(150, 150, 150));
	left_panel->SetBackgroundColour(wxColour(150, 150, 150));
	right_panel->SetBackgroundColour(wxColour(150, 150, 150));
*/

	picker = new wxDatePickerCtrl(date_panel, DPICKER, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	

	// Open file and add date entries to array

	if (!file.Exists())
		file.Create();
	file.Open();

	tokenizer.SetString(file.GetFirstLine(), ",");
	line_count = file.GetLineCount();

	// Push date from each line to vector

	for (int i = file.GetCurrentLine(); i < line_count; i++) {
		entry_dates.push_back(tokenizer.GetNextToken());
		tokenizer.SetString(file.GetNextLine());
	}
	
	// Creating the layout

	for (int i = 0; i < 10; i++) {
		label[i] = new wxStaticText(left_panel, wxID_ANY, fields[i]);
		entry[i] = new wxTextCtrl(left_panel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(50, 23), wxTE_RIGHT, wxTextValidator(wxFILTER_NUMERIC));
		gsizer1->Add(label[i], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
		gsizer1->Add(entry[i], 0, wxEXPAND);
	}

	for (int i = 10; i < 19; i++) {
		if (i == 15) {
			gsizer2->AddSpacer(23);
			gsizer2->AddSpacer(23);
		}
		label[i] = new wxStaticText(right_panel, wxID_ANY, fields[i]);
		entry[i] = new wxTextCtrl(right_panel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(50, 23), wxTE_RIGHT, wxTextValidator(wxFILTER_NUMERIC));
		gsizer2->Add(label[i], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
		gsizer2->Add(entry[i], 0, wxEXPAND);
	}

	label[DRAWER_LABEL] = new wxStaticText(left_panel, wxID_ANY, "Drawer:");
	label[DRAWER_TOTAL] = new wxStaticText(left_panel, wxID_ANY, " ");
	label[ONHAND_LABEL] = new wxStaticText(left_panel, wxID_ANY, "On Hand:");
	label[ONHAND_TOTAL] = new wxStaticText(left_panel, wxID_ANY, " ");
	label[ACTPAY_LABEL] = new wxStaticText(right_panel, wxID_ANY, "Payable:");
	label[ACTPAY_TOTAL] = new wxStaticText(right_panel, wxID_ANY, " ");
	label[DIFFER_LABEL] = new wxStaticText(right_panel, wxID_ANY, "Difference:");
	label[DIFFER_TOTAL] = new wxStaticText(right_panel, wxID_ANY, " ");

	gsizer1->Add(label[DRAWER_LABEL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer1->Add(label[DRAWER_TOTAL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer1->Add(label[ONHAND_LABEL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer1->Add(label[ONHAND_TOTAL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer2->Add(label[ACTPAY_LABEL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer2->Add(label[ACTPAY_TOTAL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer2->Add(label[DIFFER_LABEL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer2->Add(label[DIFFER_TOTAL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	wxButton* calculate_button = new wxButton(button_panel, CALCULATE, "Calculate");
	wxButton* submit_button = new wxButton(button_panel, SUBMIT, "Submit");


	// Adding stuff to things

	// Add date picker widget to date sizer
	dsizer->Add(picker);

	// Add growable columns to both grid sizers
	gsizer1->AddGrowableCol(1, 1);
	gsizer2->AddGrowableCol(1, 1);

	// Add buttons and spacers to button sizer
	bsizer->AddGrowableCol(1, 1);
	bsizer->Add(calculate_button, 0, wxALIGN_CENTER | wxALL, FromDIP(10));
	bsizer->AddSpacer(23);
	bsizer->AddSpacer(23);
	bsizer->Add(submit_button, 0, wxALIGN_CENTER | wxALL, FromDIP(10));

	// Set sizers for the various panels
	// This is what makes the panels grow with the sizers
	date_panel->SetSizer(dsizer);
	left_panel->SetSizer(gsizer1);
	right_panel->SetSizer(gsizer2);
	button_panel->SetSizer(bsizer);

	// Add panels to various sizers
	date_sizer->Add(date_panel, 1, wxEXPAND | wxALL, FromDIP(10));
	entry_sizer->Add(left_panel, 1, wxEXPAND | wxALL, FromDIP(10));
	entry_sizer->Add(right_panel, 1, wxEXPAND | wxALL, FromDIP(10));
	button_sizer->Add(button_panel, 1, wxEXPAND | wxALL, FromDIP(10));

	// Add sizers to the main panel
	main_sizer->Add(date_sizer, 0, wxEXPAND | wxALL);
	main_sizer->Add(entry_sizer, 0, wxEXPAND | wxALL);
	main_sizer->Add(button_sizer, 0, wxEXPAND | wxALL);

	// Set sizer for main panel
	// This is what makes the main panel change size with the window
	main_panel->SetSizer(main_sizer);
	main_sizer->Fit(this);

	CreateStatusBar();

}

void MainFrame::OnCalculateClicked(wxCommandEvent& evt) {
	wxDouble drawer = 0;
	wxDouble onhand = 0;
	wxDouble payable = 0;
	wxDouble difference = 0;

	new_line = "";
	date = picker->GetValue().Format("%m-%d-%Y");

	// Get values from entries
	for (int i = 0; i < 19; i++)
		entry[i]->GetValue().ToDouble(&value[i]);

	// Drawer
	for (int i = 0; i < 10; i++)
		drawer += value[i];
	label[DRAWER_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), drawer));
	new_line += (date + wxString::Format(wxT(",$%.2f"), drawer));

	// On Hand
	onhand = drawer;
	for (int i = 10; i < 15; i++)
		onhand += value[i];
	label[ONHAND_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), onhand));
	new_line += (wxString::Format(wxT(",$%.2f"), onhand));

	// Accounts Payable
	payable = value[12] + value[13];
	for (int i = 15; i < 19; i++)
		payable += value[i];
	label[ACTPAY_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), payable));
	new_line += (wxString::Format(wxT(",$%.2f"), payable));

	// Difference between Money on Hand and Accounts Payable
	difference = (onhand >= payable) ? (onhand - payable) : (payable - onhand);
	label[DIFFER_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), difference));
	new_line += (wxString::Format(wxT(",$%.2f"), difference));

	wxLogStatus(new_line);
}

void MainFrame::OnSubmitClicked(wxCommandEvent& evt) {

	// Need to push new date to vector change line_count

	for (int i = 0; i < line_count; i++) {
		if (entry_dates[i] == date) {
			line = i;
			date = entry_dates[i];
			wxMessageBox("Entry exists for date");
			break;
		}
		else {
			file.AddLine(new_line);
			wxLogStatus("Data saved to file.");
		}
	}

	

	file.Write();
	file.Close();

	
}

void MainFrame::OnDateChanged(wxDateEvent& evt) {
	date = picker->GetValue().Format("%m-%d-%Y");
	wxLogStatus("Date Changed to " + picker->GetValue().Format("%m-%d-%Y"));
}

/*
	tokenizer.SetString(file.GetFirstLine(), ",");
	line_count = file.GetLineCount();

	wxString token;

	for (line = file.GetCurrentLine(); line < line_count; line++) {

		while (tokenizer.HasMoreTokens()) {
			token += (tokenizer.GetNextToken() + ",");
		}
		file.AddLine(token);
		token = "";

		tokenizer.SetString(file.GetNextLine());
	}
*/