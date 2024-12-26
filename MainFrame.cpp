#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/vector.h>
#include <wx/numformatter.h>
#include "resource.h"

#define DRAWER_LABEL 20
#define DRAWER_TOTAL 21
#define ONHAND_LABEL 22
#define ONHAND_TOTAL 23
#define ACTPAY_LABEL 24
#define ACTPAY_TOTAL 25
#define DIFFER_LABEL 26
#define DIFFER_TOTAL 27

wxString fields[] = { "Pennies", "Nickels", "Dimes", "Quarters", "Ones", "Fives", "Tens", "Twenties", "Fifties", "Hundreds", "Checks", "Bank Acct.", "M.V. Due", "Petty Cash", "Cash Bag", "Office", "Copy", "Firearms", "Prints" };

wxCheckBox* edit;
wxVector<wxString> entryDates;
wxStaticText* label[28];
wxTextCtrl* entry[19];
wxDouble value[19];
wxDatePickerCtrl* picker;
wxString newLine;
wxButton* submitButton;
wxButton* calculateButton;
wxButton* printButton;
wxDouble drawer;
wxDouble onhand;
wxDouble payable;
wxDouble difference;
size_t line;
bool submitted;
bool clobber;

enum IDs {
	CALCULATE = 2,
	SUBMIT = 3,
	PRINT = 4,
	DPICKER = 5,
	CLOSE = 6,
	CLOBBER = 7
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)
EVT_CHECKBOX(CLOBBER, MainFrame::OnEditChecked)
EVT_BUTTON(CALCULATE, MainFrame::OnCalculateClicked)
EVT_BUTTON(SUBMIT, MainFrame::OnSubmitClicked)
EVT_BUTTON(PRINT, MainFrame::OnPrintClicked)
EVT_DATE_CHANGED(DPICKER, MainFrame::OnDateChanged)

wxEND_EVENT_TABLE()


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
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
	edit = new wxCheckBox(datePanel, CLOBBER, "Edit");

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
	dsizer->Add(picker);
	dsizer->Add(edit);

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
	//onhand = drawer;
	for (int i = 10; i < 15; i++)
		onhand += value[i];
	label[ONHAND_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), onhand));

	// Accounts Payable
	payable = value[12] + value[13];
	for (int i = 15; i < 19; i++)
		payable += value[i];
	label[ACTPAY_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), payable));

	// Difference between Money on Hand and Accounts Payable
	difference = ((drawer + onhand) >= payable) ? ((drawer + onhand) - payable) : (payable - (drawer + onhand));
	label[DIFFER_TOTAL]->SetLabel(wxString::Format(wxT("$%.2f"), difference));
	newLine += (wxString::Format(wxT(",$%.2f,$%.2f,$%.2f,$%.2f"), drawer, onhand, payable, difference));

	wxLogStatus(wxString::Format(wxT("%s, $%.2f, $%.2f, $%.2f, $%.2f"), date, drawer, onhand, payable, difference));
}

void MainFrame::OnSubmitClicked(wxCommandEvent& evt) {
	submitted = true;
	CheckDateExists();
}

void MainFrame::OnPrintClicked(wxCommandEvent& evt) {
	wxString date = picker->GetValue().Format("%m-%d-%Y");
	ShellExecute(NULL, L"open", (wxString::Format(wxT(".\\XeLaTeX\\%s.pdf"), date)), NULL, NULL, SW_SHOW);
}

void MainFrame::OnDateChanged(wxDateEvent& evt) {
	CheckDateExists();
}

void MainFrame::CheckDateExists() {
	bool dateExists = false;
	wxStringTokenizer tokenizer;
	wxString date = picker->GetValue().Format("%m-%d-%Y");
	wxTextFile file(picker->GetValue().Format("%Y.csv"));
	size_t lineCount;
	

	if (!file.Exists()) {
		file.Create();
		file.Open();
		wxMessageBox("Created a new file. Happy new year!");
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
	if ((!dateExists && !submitted) || (dateExists && clobber)) {
		calculateButton->Enable();
		submitButton->Disable();
		printButton->Disable();
		if (!dateExists) edit->Disable();
		//clobber = false;
		//edit->SetValue(clobber);

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
	//double dMonth;
	wxString date = picker->GetValue().Format("%m-%d-%Y");
	wxString sMonth = picker->GetValue().Format("%b");
	wxString sDate = picker->GetValue().Format("%d, %Y");
	wxString month = picker->GetValue().Format("%m");


	wxString year = picker->GetValue().Format("%Y");

	wxTextFile tex(".\\XeLaTeX\\Templates\\CashDrawer.tex");
	wxTextFile newTex(wxString::Format((".\\XeLaTeX\\%s.tex"), date));
	wxTextFile pdfTex(wxString::Format((".\\XeLaTeX\\%s.pdf"), date));
	wxTextFile summary(".\\XeLaTeX\\Templates\\Summary.tex");
	wxTextFile newSummary(wxString::Format((".\\XeLaTeX\\%s-%s-Summary.tex"), year, month));
	wxTextFile pdfSummary(wxString::Format((".\\XeLaTeX\\%s-%s-Summary.pdf"), year, month));
	wxString title1 = "Lincoln County Sheriff's Office";
	wxString title2 = "North Platte, Nebraska";
	size_t lineCount = 0;

	if (!newSummary.Exists()) {
		summary.Open();
		newSummary.Create();
		while (!newSummary.Open())
			continue;
		newSummary.AddLine(summary.GetFirstLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(wxString::Format((summary.GetNextLine()), year));
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.AddLine(summary.GetNextLine());
		newSummary.Write();
		summary.Close();
	}

	if (!newSummary.IsOpened()) 
		newSummary.Open();
	newSummary.InsertLine(wxString::Format(("%s %s & \\$%.2f & \\$%.2f & \\$%.2f \\\\ \\hline"), sMonth, sDate, onhand, payable, difference), (newSummary.GetLineCount() - 1));
	newSummary.Write();
	newSummary.Close();

	if (!ShellExecute(NULL, wxT("open"), (wxString::Format((".\\XeLaTeX\\tectonic.exe"))), (wxString::Format((".\\XeLaTeX\\%s-%s-Summary.tex"), year, month)), NULL, SW_HIDE)) 
		wxLogMessage("Tectonic could not start.");
	/*
	while (!newSummary.Exists() && !pdfSummary.Exists())
		continue;
	if (wxCopyFile(wxString::Format(("C:\\users\\ezeki\\source\\repos\\CashDrawer\\XeLaTeX\\%s-%s-Summary.pdf"), year, month), wxString::Format(("C:\\users\\ezeki\\source\\repos\\CashDrawer\\XeLaTeX\\PDFs\\%s-%s-Summary.pdf"), year, month)))
		wxRemoveFile(wxString::Format(("C:\\users\\ezeki\\source\\repos\\CashDrawer\\XeLaTeX\\%s-%s-Summary.pdf"), year, month));
	else {
		wxTextFile pdfSummary(wxString::Format((".\\XeLaTeX\\%s-%s-Summary.pdf"), year, month));
		wxRemoveFile(wxString::Format(("C:\\users\\ezeki\\source\\repos\\CashDrawer\\XeLaTeX\\%s-%s-Summary.pdf"), year, month));
	}
	*/

	if (!newTex.Exists()) newTex.Create();
	if (!newTex.IsOpened()) newTex.Open();
	if (!tex.IsOpened()) tex.Open();
	
	//lineCount = newTex.GetLineCount();
	//newTex.RemoveLine(lineCount - 1);
	newTex.Clear();
	
	newTex.AddLine(tex.GetFirstLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), title1, title2));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), sMonth, sDate));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[0]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[1]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[2]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[3]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[4]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[5]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[6]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[7]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[8]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[9]));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), drawer));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[10]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[11]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[12]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[13]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[14]));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), onhand));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[15]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[13]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[16]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[12]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[17]));
	newTex.AddLine(wxString::Format((tex.GetNextLine()), value[18]));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), payable));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), (drawer + onhand)));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), payable));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(wxString::Format((tex.GetNextLine()), difference));
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.AddLine(tex.GetNextLine());
	newTex.Write();
	newTex.Close();
	tex.Close();
	if (!ShellExecute(NULL, wxT("open"), wxT(".\\XeLaTeX\\tectonic.exe"), (wxString::Format(wxT(".\\XeLaTeX\\%s.tex"), date)), NULL, SW_HIDE))
		wxLogMessage("Tectonic could not start.");
	/*
	while (!newTex.Exists() && !pdfTex.Exists())
		continue;
	if (wxCopyFile(wxString::Format(("C:\\users\\ezeki\\source\\repos\\CashDrawer\\XeLaTeX\\%s.pdf"), date), wxString::Format(("C:\\users\\ezeki\\source\\repos\\CashDrawer\\XeLaTeX\\PDFs\\%s.pdf"), date))) {
		wxRemoveFile(wxString::Format(("C:\\users\\ezeki\\source\\repos\\CashDrawer\\XeLaTeX\\%s.pdf"), date));
		wxRemoveFile(wxString::Format(("C:\\users\\ezeki\\source\\repos\\CashDrawer\\XeLaTeX\\%s.tex"), date));
	}
	*/
}