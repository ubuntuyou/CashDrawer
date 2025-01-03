#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/textfile.h>
#include <wx/filefn.h>
#include <wx/tokenzr.h>
#include <wx/vector.h>
#include <wx/numformatter.h>
#include <wx/file.h>
#include "resource.h"

#define DRAWER_LABEL 20
#define DRAWER_TOTAL 21
#define ONHAND_LABEL 22
#define ONHAND_TOTAL 23
#define ACTPAY_LABEL 24
#define ACTPAY_TOTAL 25
#define DIFFER_LABEL 26
#define DIFFER_TOTAL 27

wxStaticText* label[28];
wxTextCtrl* entry[19];
wxDouble value[19];
wxDatePickerCtrl* picker;
wxCheckBox* edit;
wxButton* summaryButton;
wxButton* calculateButton;
wxButton* submitButton;
wxButton* printButton;
wxDouble drawer;
wxDouble onhand;
wxDouble payable;
wxDouble difference;
wxString newLine;
size_t line;
bool submitted;
bool clobber;



enum IDs {
	CALCULATE = 2,
	SUBMIT = 3,
	PRINT = 4,
	DPICKER = 5,
	CLOSE = 6,
	CLOBBER = 7,
	SUMMARY = 8
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)
EVT_CHECKBOX(CLOBBER, MainFrame::OnEditChecked)
EVT_BUTTON(SUMMARY, MainFrame::OnSummaryClicked)
EVT_BUTTON(CALCULATE, MainFrame::OnCalculateClicked)
EVT_BUTTON(SUBMIT, MainFrame::OnSubmitClicked)
EVT_BUTTON(PRINT, MainFrame::OnPrintClicked)
EVT_DATE_CHANGED(DPICKER, MainFrame::OnDateChanged)

wxEND_EVENT_TABLE()


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxString fields[] = { "Pennies", "Nickels", "Dimes", "Quarters", "Ones", "Fives",
		"Tens", "Twenties", "Fifties", "Hundreds", "Checks", "Bank Acct.", "M.V. Due", "Petty Cash", "Cash Bag", "Office", "Copy", "Firearms", "Prints" };

	wxIcon appIcon;
	appIcon.LoadFile("aaaa", wxBITMAP_TYPE_ICO_RESOURCE);
	if (appIcon.IsOk()) SetIcon(appIcon);

	this->SetMinClientSize(wxSize(350, 525));
	this->SetMaxClientSize(wxSize(350, 525));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* dateSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* entrySizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
	wxFlexGridSizer* gsizer1 = new wxFlexGridSizer(13, 2, 10, 8);
	wxFlexGridSizer* gsizer2 = new wxFlexGridSizer(13, 2, 10, 8);
	wxFlexGridSizer* bsizer = new wxFlexGridSizer(1, 4, 10, 10);
	wxFlexGridSizer* dsizer = new wxFlexGridSizer(1, 3, 10, 10);

	wxPanel* mainPanel = new wxPanel(this);
	wxPanel* datePanel = new wxPanel(mainPanel);
	wxPanel* leftPanel = new wxPanel(mainPanel);
	wxPanel* rightPanel = new wxPanel(mainPanel);
	wxPanel* buttonPanel = new wxPanel(mainPanel);

	
	picker = new wxDatePickerCtrl(datePanel, DPICKER, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	edit = new wxCheckBox(datePanel, CLOBBER, "Edit");
	summaryButton = new wxButton(datePanel, SUMMARY, "Summary");

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
	printButton = new wxButton(buttonPanel, PRINT, "Print");

	// Adding stuff to things

	// Add date picker widget to date sizer
	dsizer->AddGrowableCol(2, 1);
	dsizer->Add(picker, 0, wxALIGN_RIGHT | wxALL, FromDIP(10));
	dsizer->Add(edit, 0, wxALIGN_LEFT | wxALL, FromDIP(10));
	dsizer->Add(summaryButton, 0, wxALIGN_RIGHT | wxALL, FromDIP(10));

	// Add growable columns to both grid sizers
	gsizer1->AddGrowableCol(1, 1);
	gsizer2->AddGrowableCol(1, 1);

	// Add buttons and spacers to button sizer
	bsizer->AddGrowableCol(1, 1);
	bsizer->Add(calculateButton, 0, wxALIGN_CENTER | wxALL, FromDIP(10));
	bsizer->AddSpacer(23);
	bsizer->Add(submitButton, 0, wxALIGN_CENTER | wxALL, FromDIP(10));
	bsizer->Add(printButton, 0, wxALIGN_CENTER | wxALL, FromDIP(10));

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
}

void MainFrame::OnClose(wxCloseEvent& evt) {
	Destroy();
}

void MainFrame::OnEditChecked(wxCommandEvent& evt) {
	if (evt.IsChecked())
		clobber = true;
	else
		clobber = false;
	CheckDateExists();
}

void MainFrame::OnCalculateClicked(wxCommandEvent& evt) {
	wxString date = picker->GetValue().Format("%m-%d-%Y");
	drawer = 0;
	onhand = 0;
	payable = 0;
	difference = 0;

	submitButton->Enable();
	//edit->SetValue(false);

	// Get values from entries
	newLine = date;

	for (int i = 0; i < 19; i++) {
		entry[i]->GetValue().ToDouble(&value[i]);
		newLine += wxString::Format(wxT(",%.2f"), value[i]);
	}

	// Drawer
	for (int i = 0; i < 10; i++)
		drawer += value[i];
	label[DRAWER_TOTAL]->SetLabel(wxString::Format(wxT("%.2f"), drawer));

	// On Hand
	//onhand = drawer;
	for (int i = 10; i < 15; i++)
		onhand += value[i];
	label[ONHAND_TOTAL]->SetLabel(wxString::Format(wxT("%.2f"), onhand));

	// Accounts Payable
	payable = value[12] + value[13];
	for (int i = 15; i < 19; i++)
		payable += value[i];
	label[ACTPAY_TOTAL]->SetLabel(wxString::Format(wxT("%.2f"), payable));

	// Difference between Money on Hand and Accounts Payable
	difference = ((drawer + onhand) >= payable) ? ((drawer + onhand) - payable) : (payable - (drawer + onhand));
	label[DIFFER_TOTAL]->SetLabel(wxString::Format(wxT("%.2f"), difference));
	newLine += (wxString::Format(wxT(",%.2f,%.2f,%.2f,%.2f"), drawer, onhand, payable, difference));

	wxLogStatus(wxString::Format(wxT("%s, $%.2f, $%.2f, $%.2f, $%.2f"), date, drawer, onhand, payable, difference));
}

void MainFrame::OnSubmitClicked(wxCommandEvent& evt) {
	submitted = true;
	CheckDateExists();
}

void MainFrame::OnSummaryClicked(wxCommandEvent& evt) {
	wxString date = picker->GetValue().Format("%Y-%m");
	
	if (wxFile().Exists(wxString::Format((".\\summary\\%s-Summary.pdf"), date)))
		wxLaunchDefaultApplication(wxString::Format((".\\summary\\%s-Summary.pdf"), date));
	else
		wxLogStatus(wxString::Format(("No summary exists for %s"), date));
}

void MainFrame::OnPrintClicked(wxCommandEvent& evt) {
	wxString date = picker->GetValue().Format("%m-%d-%Y");
	if (wxFile().Exists(wxString::Format((".\\report\\%s.pdf"), date)))
		wxLaunchDefaultApplication(wxString::Format((".\\report\\%s.pdf"), date));
	else
		wxLogStatus(wxString::Format(("No report exists for %s"), date));
}

void MainFrame::OnDateChanged(wxDateEvent& evt) {
	CheckDateExists();
}

void MainFrame::CheckDateExists() {
	wxVector<wxString> entryDates;
	wxStringTokenizer tokenizer;
	wxString date = picker->GetValue().Format("%m-%d-%Y");
	wxTextFile file(picker->GetValue().Format(".\\data\\%Y.csv"));
	size_t lineCount;
	bool dateExists = false;

	if (!file.Exists()) {
		file.Create();
		file.Open();
		//wxMessageBox("Created a new file.");
	}
	if (!file.IsOpened()) {
		file.Open();
	}

	lineCount = file.GetLineCount();

	tokenizer.SetString(file.GetFirstLine(), ",");

	entryDates.clear();

	for (int i = file.GetCurrentLine(); i < lineCount; i++) {
		entryDates.push_back(tokenizer.GetNextToken());
		tokenizer.SetString(file.GetNextLine(), ",");
	}

	for (int i = 0; i < lineCount; i++) {
		if (entryDates[i] == date) {
			dateExists = true;
			line = i;

			calculateButton->Disable();
			submitButton->Disable();
			printButton->Enable();
			edit->Enable();

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

			wxLogStatus("Entry exists for date");	
			break;
		}
	}
	if (!dateExists && !submitted) {
		calculateButton->Enable();
		submitButton->Disable();
		printButton->Disable();

		for (int i = 0; i < 19; i++) {
			entry[i]->SetValue("0.00");
			entry[i]->SetEditable(true);
		}
		label[DRAWER_TOTAL]->SetLabel(" ");
		label[ONHAND_TOTAL]->SetLabel(" ");
		label[ACTPAY_TOTAL]->SetLabel(" ");
		label[DIFFER_TOTAL]->SetLabel(" ");
		
		wxLogStatus(" ");
	}
	if (dateExists && clobber) {
		calculateButton->Enable();
		submitButton->Disable();
		printButton->Disable();

		for (int i = 0; i < 19; i++) {
			entry[i]->SetEditable(true);
		}

		wxLogStatus(" ");
	}
	if ((!dateExists && submitted) || (submitted && clobber)) {
		if (clobber)
			file.RemoveLine(line);

		submitted = false;
		calculateButton->Disable();
		submitButton->Disable();
		printButton->Enable();
		clobber = false;
		edit->SetValue(clobber);
		edit->Enable();

		file.AddLine(newLine);
		file.Write();

		tokenizer.SetString(newLine, ",");
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

		CreatePDF();

		wxLogStatus("Data saved to file.");
	}
}

void MainFrame::CreatePDF() {
	wxString date = picker->GetValue().Format("%m-%d-%Y");
	wxString sMonth = picker->GetValue().Format("%b");
	wxString lMonth = picker->GetValue().Format("%B");
	wxString sDate = picker->GetValue().Format("%d, %Y");
	wxString month = picker->GetValue().Format("%m");
	wxString year = picker->GetValue().Format("%Y");
	wxVector<wxString> sumDates;
	wxStringTokenizer tokenizer;
	bool dateExists = false;

	wxTextFile report(".\\template\\Report.tex");
	wxTextFile newReport(wxString::Format((".\\data\\daily\\%s.tex"), date));
	wxTextFile summary(".\\template\\Summary.tex");
	wxTextFile newSummary(wxString::Format((".\\data\\monthly\\%s-%s-Summary.tex"), year, month));
	wxString title1 = "Lincoln County Sheriff's Office";
	wxString title2 = "North Platte, Nebraska";
	size_t lineCount = 0;
	size_t line = 0;

	if (!newSummary.Exists()) {
		summary.Open();
		newSummary.Create();
		while (!newSummary.Open())
			continue;
		newSummary.AddLine(summary.GetFirstLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(wxString::Format((summary.GetNextLine()), sMonth, year));
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.Write();
		summary.Close();
	}

	if (!newSummary.IsOpened()) 
		newSummary.Open();

	// Check if date exists and replace if it does
	sumDates.clear();
	lineCount = newSummary.GetLineCount();
	tokenizer.SetString(newSummary.GetFirstLine(), " &");

	for (int i = 0; i < lineCount; i++) {
		sumDates.push_back(tokenizer.GetNextToken());
		tokenizer.SetString(newSummary.GetNextLine(), " &");
	}

	for (int i = 0; i < lineCount; i++) {
		if (sumDates[i] == date) {
			line = i;
			dateExists = true;
			break;
		}
		else
			line = lineCount - 1;
	}
	
	newSummary.InsertLine(wxString::Format(("%s & \\$%.2f & \\$%.2f & \\$%.2f \\\\ \\hline"), date, onhand, payable, difference), line);
	if (dateExists)
		newSummary.RemoveLine(line + 1);

	newSummary.Write();
	newSummary.Close();

	if (!wxExecute(wxString::Format((".\\tectonic.exe .\\data\\monthly\\%s-%s-Summary.tex -c minimal -o .\\summary"), year, month)))
		wxLogMessage("Error generating summary!");

	if (!newReport.Exists()) newReport.Create();
	if (!newReport.IsOpened()) newReport.Open();
	if (!report.IsOpened()) report.Open();
	
	newReport.Clear();
	
	newReport.AddLine(report.GetFirstLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), title1, title2));
	newReport.AddLine(wxString::Format((report.GetNextLine()), sMonth, sDate));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[0]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[1]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[2]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[3]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[4]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[5]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[6]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[7]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[8]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[9]));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), drawer));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[10]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[11]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[12]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[13]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[14]));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), onhand));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[15]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[13]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[16]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[12]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[17]));
	newReport.AddLine(wxString::Format((report.GetNextLine()), value[18]));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), payable));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), (drawer + onhand)));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), payable));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(wxString::Format((report.GetNextLine()), difference));
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.AddLine(report.GetNextLine());
	newReport.Write();
	newReport.Close();
	report.Close();
	
	if (!wxExecute(wxString::Format((".\\tectonic.exe .\\data\\daily\\%s.tex -c minimal -o .\\report"), date)))
		wxLogMessage("Error generating report!");
}