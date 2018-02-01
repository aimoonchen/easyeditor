#include "main.h"
#include "Task.h"

#include <ee/Frame.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	auto frame = new ee::Frame("EasyOne", "one", wxDefaultSize, true);
	auto task = new eone::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}