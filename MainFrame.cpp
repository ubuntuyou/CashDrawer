#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/file.h>
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

wxVector<wxString> entryDates;
wxStaticText* label[28];
wxTextCtrl* entry[19];
wxDouble value[19];
wxDatePickerCtrl* picker;
wxString date;
wxString newLine;
size_t lineCount;
size_t line;
wxStringTokenizer tokenizer;
wxTextFile file(wxDateTime::Today().Format("%Y.csv"));
wxButton* submitButton;
wxButton* calculateButton;
bool dateExists;

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

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* dateSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* entrySizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
	wxFlexGridSizer* gsizer1 = new wxFlexGridSizer(13, 2, 10, 8);
	wxFlexGridSizer* gsizer2 = new wxFlexGridSizer(13, 2, 10, 8);
	wxFlexGridSizer* bsizer = new wxFlexGridSizer(1, 4, 10, 10);
	wxFlexGridSizer* dsizer = new wxFlexGridSizer(1, 2, 10, 10);

	wxPanel* mainPanel = new wxPanel(this);
	wxPanel* datePanel = new wxPanel(mainPanel);
	wxPanel* leftPanel = new wxPanel(mainPanel);
	wxPanel* rightPanel = new wxPanel(mainPanel);
	wxPanel* buttonPanel = new wxPanel(mainPanel);

/*
	mainPanel->SetBackgroundColour(wxColour(100, 100, 100));
	datePanel->SetBackgroundColour(wxColour(150, 150, 150));
	buttonPanel->SetBackgroundColour(wxColour(150, 150, 150));
	leftPanel->SetBackgroundColour(wxColour(150, 150, 150));
	rightPanel->SetBackgroundColour(wxColour(150, 150, 150));
*/

	picker = new wxDatePickerCtrl(datePanel, DPICKER, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);


	// Creating the layout

	for (int i = 0; i < 10; i++) {
		label[i] = new wxStaticText(leftPanel, wxID_ANY, fields[i]);
		entry[i] = new wxTextCtrl(leftPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(50, 23), wxTE_RIGHT, wxTextValidator(wxFILTER_NUMERIC));
		gsizer1->Add(label[i], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
		gsizer1->Add(entry[i], 0, wxEXPAND);
	}

	for (int i = 10; i < 19; i++) {
		if (i == 15) {
			gsizer2->AddSpacer(23);
			gsizer2->AddSpacer(23);
		}
		label[i] = new wxStaticText(rightPanel, wxID_ANY, fields[i]);
		entry[i] = new wxTextCtrl(rightPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(50, 23), wxTE_RIGHT, wxTextValidator(wxFILTER_NUMERIC));
		gsizer2->Add(label[i], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
		gsizer2->Add(entry[i], 0, wxEXPAND);
	}

	label[DRAWER_LABEL] = new wxStaticText(leftPanel, wxID_ANY, "Drawer:");
	label[DRAWER_TOTAL] = new wxStaticText(leftPanel, wxID_ANY, " ");
	label[ONHAND_LABEL] = new wxStaticText(leftPanel, wxID_ANY, "On Hand:");
	label[ONHAND_TOTAL] = new wxStaticText(leftPanel, wxID_ANY, " ");
	label[ACTPAY_LABEL] = new wxStaticText(rightPanel, wxID_ANY, "Payable:");
	label[ACTPAY_TOTAL] = new wxStaticText(rightPanel, wxID_ANY, " ");
	label[DIFFER_LABEL] = new wxStaticText(rightPanel, wxID_ANY, "Difference:");
	label[DIFFER_TOTAL] = new wxStaticText(rightPanel, wxID_ANY, " ");

	gsizer1->Add(label[DRAWER_LABEL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer1->Add(label[DRAWER_TOTAL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer1->Add(label[ONHAND_LABEL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer1->Add(label[ONHAND_TOTAL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer2->Add(label[ACTPAY_LABEL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer2->Add(label[ACTPAY_TOTAL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer2->Add(label[DIFFER_LABEL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	gsizer2->Add(label[DIFFER_TOTAL], 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	calculateButton = new wxButton(buttonPanel, CALCULATE, "Calculate");
	submitButton = new wxButton(buttonPanel, SUBMIT, "Submit");
	


	// Adding stuff to things

	// Add date picker widget to date sizer
	dsizer->Add(picker);

	// Add growable columns to both grid sizers
	gsizer1->AddGrowableCol(1, 1);
	gsizer2->AddGrowableCol(1, 1);

	// Add buttons and spacers to button sizer
	bsizer->AddGrowableCol(1, 1);
	bsizer->Add(calculateButton, 0, wxALIGN_CENTER | wxALL, FromDIP(10));
	bsizer->AddSpacer(23);
	bsizer->AddSpacer(23);
	bsizer->Add(submitButton, 0, wxALIGN_CENTER | wxALL, FromDIP(10));

	// Set sizers for the various panels
	// This is what makes the panels grow with the sizers
	datePanel->SetSizer(dsizer);
	leftPanel->SetSizer(gsizer1);
	rightPanel->SetSizer(gsizer2);
	buttonPanel->SetSizer(bsizer);

	// Add panels to various sizers
	dateSizer->Add(datePanel, 1, wxEXPAND | wxALL, FromDIP(10));
	entrySizer->Add(leftPanel, 1, wxEXPAND | wxALL, FromDIP(10));
	entrySizer->Add(rightPanel, 1, wxEXPAND | wxALL, FromDIP(10));
	buttonSizer->Add(buttonPanel, 1, wxEXPAND | wxALL, FromDIP(10));

	// Add sizers to the main panel
	mainSizer->Add(dateSizer, 0, wxEXPAND | wxALL);
	mainSizer->Add(entrySizer, 0, wxEXPAND | wxALL);
	mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL);

	// Set sizer for main panel
	// This is what makes the main panel change size with the window
	mainPanel->SetSizer(mainSizer);
	mainSizer->Fit(this);

	CreateStatusBar();

	
	CheckDateExists();
	if (dateExists) {
		calculateButton->Disable();
		submitButton->Disable();
		tokenizer.SetString(file.GetLine(line), ",");
		tokenizer.GetNextToken();

		while (tokenizer.HasMoreTokens()) {
			for (int i = 0; i < 19; i++) {
				entry[i]->SetValue(tokenizer.GetNextToken());
				entry[i]->SetEditable(false);
			}
			label[DRAWER_TOTAL]->SetLabel(tokenizer.GetNextToken());
			label[ONHAND_TOTAL]->SetLabel(tokenizer.GetNextToken());
			label[ACTPAY_TOTAL]->SetLabel(tokenizer.GetNextToken());
			label[DIFFER_TOTAL]->SetLabel(tokenizer.GetNextToken());
		}
	}
	else {
		calculateButton->Enable();
		submitButton->Disable();
	}
}

void MainFrame::OnCalculateClicked(wxCommandEvent& evt) {
	wxDouble drawer = 0;
	wxDouble onhand = 0;
	wxDouble payable = 0;
	wxDouble difference = 0;

	submitButton->Enable();
	
	date = picker->GetValue().Format("%m-%d-%Y");


	// Get values from entries
	newLine = date;

	for (int i = 0; i < 19; i++) {
		entry[i]->GetValue().ToDouble(&value[i]);
		newLine += wxString::Format(wxT(",$%.2f"), value[i]);
	}

	// Drawer
	for (int i = 0; i < 10; i++)
		drawer += value[i];
	label[DRAWER_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), drawer));

	// On Hand
	onhand = drawer;
	for (int i = 10; i < 15; i++)
		onhand += value[i];
	label[ONHAND_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), onhand));

	// Accounts Payable
	payable = value[12] + value[13];
	for (int i = 15; i < 19; i++)
		payable += value[i];
	label[ACTPAY_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), payable));

	// Difference between Money on Hand and Accounts Payable
	difference = (onhand >= payable) ? (onhand - payable) : (payable - onhand);
	label[DIFFER_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), difference));
	newLine += (wxString::Format(wxT(",$%.2f,$%.2f,$%.2f,$%.2f"), drawer, onhand, payable, difference));

	wxLogStatus(wxString::Format(wxT("%s, $%.2f, $%.2f, $%.2f, $%.2f"), date, drawer, onhand, payable, difference));
}

void MainFrame::OnSubmitClicked(wxCommandEvent& evt) {
	// Need to make overwrite confirmation
	// If overwrite confirmed then insert new line and delete the old one
	// Maybe sort entries by date by pushing them all to a vector and sorting 
	// before clearing file and rewriting from vector

	if (!dateExists) {
		file.AddLine(newLine);
		lineCount = file.GetLineCount();
		file.Write();
		file.Close();
		wxLogStatus("Data saved to file.");
		CheckDateExists();
		calculateButton->Disable();
		submitButton->Disable();

		tokenizer.SetString(file.GetLine(line), ",");
		tokenizer.GetNextToken();

		while (tokenizer.HasMoreTokens()) {
			for (int i = 0; i < 19; i++) {
				entry[i]->SetValue(tokenizer.GetNextToken());
				entry[i]->SetEditable(false);
			}
			label[DRAWER_TOTAL]->SetLabel(tokenizer.GetNextToken());
			label[ONHAND_TOTAL]->SetLabel(tokenizer.GetNextToken());
			label[ACTPAY_TOTAL]->SetLabel(tokenizer.GetNextToken());
			label[DIFFER_TOTAL]->SetLabel(tokenizer.GetNextToken());
		}
	}
}

void MainFrame::OnDateChanged(wxDateEvent& evt) {
	CheckDateExists(); // Sets dateExists to true or false
	
	if (dateExists) {
		calculateButton->Disable();
		submitButton->Disable();
		tokenizer.SetString(file.GetLine(line), ",");
		tokenizer.GetNextToken();
		
		while (tokenizer.HasMoreTokens()) {
			for (int i = 0; i < 19; i++) {
				entry[i]->SetValue(tokenizer.GetNextToken());
				entry[i]->SetEditable(false);
			}
			label[DRAWER_TOTAL]->SetLabel(tokenizer.GetNextToken());
			label[ONHAND_TOTAL]->SetLabel(tokenizer.GetNextToken());
			label[ACTPAY_TOTAL]->SetLabel(tokenizer.GetNextToken());
			label[DIFFER_TOTAL]->SetLabel(tokenizer.GetNextToken());
		}
	}
	else {
		calculateButton->Enable();
		submitButton->Disable();
		for (int i = 0; i < 19; i++) {
			entry[i]->SetValue("0.00");
			entry[i]->SetEditable(true);
		}
		label[DRAWER_TOTAL]->SetLabel(" ");
		label[ONHAND_TOTAL]->SetLabel(" ");
		label[ACTPAY_TOTAL]->SetLabel(" ");
		label[DIFFER_TOTAL]->SetLabel(" ");
	}
}

void MainFrame::CheckDateExists() {
	dateExists = false;

	if (!file.Exists()) {
		file.Create();
		wxMessageBox("Created a new file. Happy new year!");
	}
	if (!file.IsOpened()) {
		file.Open();
	}

	date = picker->GetValue().Format("%m-%d-%Y");

	tokenizer.SetString(file.GetFirstLine(), ",");
	lineCount = file.GetLineCount();

	entryDates.clear();

	for (int i = file.GetCurrentLine(); i < lineCount; i++) {
		entryDates.push_back(tokenizer.GetNextToken());
		tokenizer.SetString(file.GetNextLine(), ",");
	}
	
	for (int i = 0; i < lineCount; i++) {
		if (entryDates[i] == date) {
			wxLogStatus("Entry exists for date");
			dateExists = true;
			line = i;
			break;
		}
		else
			wxLogStatus(" ");
	}
}

/*
	tokenizer.SetString(file.GetFirstLine(), ",");
	lineCount = file.GetLineCount();

	wxString token;

	for (line = file.GetCurrentLine(); line < lineCount; line++) {

		while (tokenizer.HasMoreTokens()) {
			token += (tokenizer.GetNextToken() + ",");
		}
		file.AddLine(token);
		token = "";

		tokenizer.SetString(file.GetNextLine());
	}
*/