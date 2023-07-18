#include <wx/wx.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/filename.h>
#include <fstream>

int width = 500;
int height = 400;

class Window : public wxFrame {
public:
	Window(const wxString windowTitle);

	wxMenuBar* MenuBar;
	wxMenu* MenuObjects;
	const int OpenId = wxNewId();
	const int SaveId = wxNewId();

	void OnSave(wxCommandEvent& event, wxTextCtrl* input, wxStaticText* file_title) {
		wxString textInput = input->GetValue(); 
		wxFileDialog* saveFileDialog = new wxFileDialog(this, wxT("Save"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		
		if (saveFileDialog->ShowModal() == wxID_CANCEL) {
			return;
		}

		wxString file_name = saveFileDialog->GetPath();
		
		wxFileName FileName(file_name);
		wxString file = FileName.GetFullName();

		wxFile createFile;
		if (createFile.Create(file_name, true)) {
			createFile.Write(textInput);
			createFile.Close();
		}

		file_title->SetLabel(file);
	}

	void OnOpen(wxCommandEvent& event, wxTextCtrl* input, wxStaticText* file_title) {
		wxString read_text;
		wxFileDialog* openFileDialog = new wxFileDialog(this, wxT("Open"), wxEmptyString, wxEmptyString, wxT(""), wxFD_OPEN);

		if (openFileDialog->ShowModal() == wxID_CANCEL) {
			return;
		}

		wxString open_file = openFileDialog->GetPath();
		
		wxFileName FileName(open_file);
		wxString file_name = FileName.GetFullName();

		wxTextFile OpenFile;
		 if (OpenFile.Open(open_file)) {
		    for (size_t lineIdx = 0; lineIdx < OpenFile.GetLineCount(); ++lineIdx) {
	            read_text += OpenFile.GetLine(lineIdx);
	            if (lineIdx != OpenFile.GetLineCount() - 1) {
	                read_text += "\n";  // Add a newline between lines
	            }
	        }
		    
		    OpenFile.Close();
	    }		

		input->ChangeValue(read_text);
		file_title->SetLabel(file_name);
	}

	void OnRun(wxCommandEvent& event, wxStaticText* file) {
		wxString file_path = file->GetLabel();
		wxMessageBox(file_path);
	}
};

class App : public wxApp {
public:
	virtual bool OnInit();
};

Window::Window(const wxString windowTitle)
	: wxFrame(NULL, wxID_ANY, windowTitle, wxDefaultPosition, wxSize(width, height)) {
		wxPanel* panel = new wxPanel(this, wxID_ANY);

		MenuBar = new wxMenuBar();
		MenuObjects = new wxMenu();

		wxMenuItem* OpenFile = new wxMenuItem(MenuObjects, OpenId, "&Open");
		wxMenuItem* SaveFile = new wxMenuItem(MenuObjects, SaveId, "&Save");
		MenuObjects->Append(wxID_OPEN, "&Open");
		MenuObjects->Append(wxID_SAVE, "&Save");

		wxStaticText* fileTitle = new wxStaticText(panel, wxID_ANY, wxT("Untitled"), wxPoint(150, 5));

		wxTextCtrl* inputField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(20, 30), wxSize(300, 280), wxTE_MULTILINE);

		Bind(wxEVT_MENU, [=](wxCommandEvent& event) {Window::OnSave(event, inputField, fileTitle);}, wxID_SAVE);
		Bind(wxEVT_MENU, [=](wxCommandEvent& event) {Window::OnOpen(event, inputField, fileTitle);}, wxID_OPEN);

		MenuBar->Append(MenuObjects, "File");
		SetMenuBar(MenuBar);

		wxButton* saveButton = new wxButton(panel, wxID_ANY, wxT("Save"), wxPoint(370, 30));
		wxButton* openButton = new wxButton(panel, wxID_ANY, wxT("Open"), wxPoint(370, 90));
		wxButton* runButton = new wxButton(panel, wxID_ANY, wxT("Run"), wxPoint(370, 150));
		saveButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {Window::OnSave(event, inputField, fileTitle);});
		openButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {Window::OnOpen(event, inputField, fileTitle);});
		runButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {Window::OnRun(event, fileTitle);});

	}




		// if (wxFileExists(file_name)) {
		// 	wxMessageDialog* overwrite_prompt = new wxMessageDialog(NULL, wxT("Overwrite the file contents?"), wxT("Overwrite"), wxYES_NO | wxICON_EXCLAMATION);
		// 	if (overwrite_prompt->ShowModal() == wxID_NO) {
		// 		return;
		// 	}
		// }
