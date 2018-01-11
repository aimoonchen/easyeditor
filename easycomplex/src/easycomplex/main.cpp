#include "main.h"
#include "Task.h"
#include "Frame.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyanim2.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyshape.h>
#include <easyicon.h>
#include <easyshadow.h>
#include <easytext.h>
#include <easymask.h>
#include <easytrail.h>
#include <easyskeleton.h>
#include <easyaudio.h>
#include <easynode3d.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ANIMATION, &libanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ANIMATION, &libanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ANIM2, &libanim2::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ANIM2, &libanim2::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SCALE9, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SCALE9, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_MESH, &emesh::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MESH, &emesh::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Sprite::Create);

	//ee::SymbolFactory::RegisterCreator(ee::SYM_TERRAIN2D, &eterrain2d::Symbol::Create);
	//ee::SpriteFactory::Instance()->RegisterCreator(ee::SYM_TERRAIN2D, &eterrain2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TEXTURE, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TEXTURE, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SHAPE, &eshape::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SHAPE, &eshape::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ICON, &eicon::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ICON, &eicon::Sprite::Create);

	//ee::SymbolFactory::RegisterCreator(ee::SYM_SHADOW, &eshadow::Symbol::Create);
	//ee::SpriteFactory::Instance()->RegisterCreator(ee::SYM_SHADOW, &eshadow::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TEXTBOX, &etext::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TEXTBOX, &etext::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_MASK, &emask::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MASK, &emask::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TRAIL, &etrail::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TRAIL, &etrail::Sprite::Create);

	//ee::SymbolFactory::RegisterCreator(s2::SYM_SKELETON, &libskeleton::Symbol::Create);
	//ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SKELETON, &libskeleton::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_AUDIO, &eaudio::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_AUDIO, &eaudio::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_MODEL, &enode3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MODEL, &enode3d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ecomplex::Frame("EasyComplex", ee::SymbolFile::Instance()->Tag(s2::SYM_COMPLEX));
	ecomplex::Task* task = new ecomplex::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		const_cast<ee::EditPanel*>(task->GetEditPanel())->GetCanvas()->SetCurrentCanvas();
		std::string path(wxGetApp().argv[1]);
		frame->InitWithFile(path);
	}

	return true;
}