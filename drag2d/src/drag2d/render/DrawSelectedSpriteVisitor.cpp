#include "DrawSelectedSpriteVisitor.h"
#include "PrimitiveDraw.h"

#include "dataset/AbstractBV.h"
#include "dataset/ISprite.h"
#include "dataset/ImageSprite.h"
#include "dataset/Image.h"
#include "render/style_config.h"
#include "render/GL10.h"
#include "common/Matrix.h"
#include "common/Config.h"
#include "common/SettingData.h"

namespace d2d
{

DrawSelectedSpriteVisitor::DrawSelectedSpriteVisitor(const Colorf& color)
	: m_color(color)
{}

void DrawSelectedSpriteVisitor::visit(Object* object, bool& bFetchNext) 
{
	std::vector<Vector> bound;
	ISprite* sprite = static_cast<ISprite*>(object);
	sprite->getBounding()->getBoundPos(bound);

	PrimitiveDraw::drawPolyline(bound, m_color, true);

	// todo: bad
	if (Config::Instance()->GetSettings().visible_image_edge)
	{
		if (ImageSprite* s = dynamic_cast<ImageSprite*>(sprite))
		{
			GL10::PushMatrix();

			Matrix mt;
			s->GetTransMatrix(mt);
			GL10::MultMatrixf((const float*)mt.getElements( ));

			Image* img = s->getSymbol().getImage();
			PrimitiveDraw::rect(Vector(0, 0), img->originWidth() * 0.5f, 
				img->originHeight() * 0.5f, LIGHT_GREY_THIN_LINE);

			GL10::PopMatrix();
		}
	}

	bFetchNext = true;
}

} // d2d