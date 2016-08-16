#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "SymbolContainer.h"

#include <easyshape.h>

#include <ee/SelectShapesOP.h>

namespace etexture
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl(new SymbolContainer(m_symbol = new Symbol))
{
	SetEditOP(new ee::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, ee::Sprite* edited, 
					   const ee::MultiSpritesImpl* bg_sprites, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl(new SymbolContainer(dynamic_cast<Symbol*>(edited->GetSymbol())))
{
	ee::EditOP* op = new eshape::EditPolylineOP<eshape::DrawPolygonOP, ee::SelectShapesOP>(this, 
		GetStageImpl(), this, NULL, new ee::OneFloatValueStatic(5), NULL);
	SetEditOP(op);
	op->RemoveReference();

	StageCanvas* canvas = new StageCanvas(this, glctx, edited, bg_sprites);
	SetCanvas(canvas);

	m_symbol = dynamic_cast<Symbol*>(edited->GetSymbol());
	if (m_symbol) {
		m_symbol->AddReference();
	}
}

StagePanel::~StagePanel()
{
	m_symbol->RemoveReference();
}

}