#include "main.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Frame.h>

#include <easyanim3d.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_MODEL, &eanim3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MODEL, &eanim3d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ee::Frame("EasyAnim3D", eanim3d::FILE_TAG);
	eanim3d::Task* task = new eanim3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}