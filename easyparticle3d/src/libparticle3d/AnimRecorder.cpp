#include "AnimRecorder.h"

#include <ee/DummySprite.h>
#include <ee/DummySymbol.h>

#include <easyanim.h>

#include <sprite2/RenderColor.h>

namespace eparticle3d
{

static const int MAX_FRAME = 1024;

AnimRecorder::AnimRecorder(int max_per_frame)
	: m_curr_frame(NULL)
	, m_frame_pool(MAX_FRAME)
	, m_item_pool(MAX_FRAME * max_per_frame)
{
}

AnimRecorder::~AnimRecorder()
{
	Clear();
}

void AnimRecorder::AddItem(const std::string& filepath, float x, float y, float angle, 
						   float scale, const s2::Color& mul_col, const s2::Color& add_col)
{
	while (!m_curr_frame) {
		m_curr_frame = m_frame_pool.GetPointer();
//		assert(m_curr_frame);
		if (!m_curr_frame) {
			Clear();
		} else {
			m_curr_frame->items.clear();
		}
	}

	Item* item = m_item_pool.GetPointer();
	assert(item);
	item->filepath = filepath;
	item->x = x;
	item->y = y;
	item->angle = angle;
	item->scale = scale;
	item->mul_col = mul_col;
	item->add_col = add_col;
	m_curr_frame->items.push_back(item);
}

void AnimRecorder::FinishFrame()
{
	if (m_curr_frame && !m_curr_frame->items.empty()) {
		m_frames.push_back(m_curr_frame);
		m_curr_frame = NULL;
	}
}

void AnimRecorder::Clear()
{
	m_frames.clear();
	m_curr_frame = NULL;

	m_frame_pool.Reset();
	m_item_pool.Reset();
}

void AnimRecorder::StoreToFile(const std::string& filepath) const
{
	libanim::Symbol* sym = new libanim::Symbol;
	auto layer = mm::allocate_unique<s2::AnimSymbol::Layer>();
	// sym->name = ani->export_name;
	sym->SetFPS(30);
	// particle 60fps, while anim 30fps
	for (int i = 0, n = m_frames.size(); i*2 < n; ++i)
//	for (int i = 0, n = m_frames.size(); i < n; ++i)
	{
		auto frame = mm::allocate_unique<s2::AnimSymbol::Frame>();
		frame->index = i + 1;
		frame->tween = false;
		Frame* record_frame = m_frames[i*2];
//		Frame* record_frame = m_frames[i];
		for (int j = 0, m = record_frame->items.size(); j < m; ++j)
		{
			Item* item = record_frame->items[j];
			auto spr = std::make_shared<ee::DummySprite>(
				std::make_shared<ee::DummySymbol>(item->filepath));

			spr->SetPosition(sm::vec2(item->x, item->y));
			spr->SetAngle(item->angle);
			spr->SetScale(sm::vec2(item->scale, item->scale));
			s2::RenderColor rc = spr->GetColor();
			rc.SetMul(item->mul_col);
			spr->SetColor(rc);

			frame->sprs.push_back(spr);
		}
		layer->frames.push_back(std::move(frame));
	}
	sym->AddLayer(std::move(layer));

	libanim::FileSaver::Store(filepath.c_str(), *sym);
	delete sym;
}

//////////////////////////////////////////////////////////////////////////
// AnimRecorder::Frame
//////////////////////////////////////////////////////////////////////////

AnimRecorder::Frame::~Frame()
{
//	Clear();

	items.clear();
}

// void AnimRecorder::Frame::Clear()
// {
// 	items.clear();
// }

}