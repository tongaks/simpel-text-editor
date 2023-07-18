#include <wx/wx.h>
#include "text_edit.h"

IMPLEMENT_APP(App)

bool App::OnInit() {
	Window* window = new Window(wxT("Text Editor"));
	window->Show(true);

	return true;
}