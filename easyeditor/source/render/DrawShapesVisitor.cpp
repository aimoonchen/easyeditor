#include "DrawShapesVisitor.h"
#include "Shape.h"
#include "SettingData.h"
#include "Config.h"

#include <SM_Test.h>
#include <gum/GTxt.h>

namespace ee
{

DrawShapesVisitor::DrawShapesVisitor(const sm::rect& screen_region, float cam_scale)
	: m_screen_region(screen_region)
	, m_cam_scale(cam_scale)
{
}

void DrawShapesVisitor::Visit(const std::shared_ptr<Shape>& shape, bool& next)
{
	next = true;

	if (!shape) {
		return;
	}

	const sm::rect& r = shape->GetBounding();
	if (m_screen_region.IsValid() && !sm::is_rect_intersect_rect(r, m_screen_region)) {
		return;
	}

	shape->Draw(m_rp);

	ee::SettingData& cfg = ee::Config::Instance()->GetSettings();
	if (cfg.visible_node_name) 
	{
		sm::vec2 center = r.Center();
		S2_MAT mt;
		float s = std::max(1.0f, m_cam_scale) * cfg.node_name_scale;
		mt.Scale(s, s);
		mt.Translate(center.x, center.y);
		gum::GTxt::Instance()->Draw(mt, shape->GetName().c_str());
	}
}

}