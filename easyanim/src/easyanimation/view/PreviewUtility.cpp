#include "PreviewUtility.h"

#include "dataset/DataMgr.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"

namespace eanim
{

void PreviewUtility::GetCurrSprites(const d2d::PlayControl& ctrl, std::vector<d2d::ISprite*>& sprites)
{
	int frame_idx = ctrl.frame();
	for (size_t i = 0, n = DataMgr::Instance()->GetLayers().Size(); i < n; ++i)
	{
		Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(i);

		KeyFrame *curr_f = layer->GetCurrKeyFrame(frame_idx),
			*next_f = layer->GetNextKeyFrame(frame_idx);
		if (!curr_f)
			continue;

		if (!curr_f) 
		{
			for (int i = 0, n = curr_f->Size(); i < n; ++i) {
				sprites.push_back(curr_f->GetSprite(i)->Clone());
			}
		}
		else if (!curr_f->HasClassicTween() || !next_f)
		{
			for (int i = 0, n = curr_f->Size(); i < n; ++i) {
				sprites.push_back(curr_f->GetSprite(i)->Clone());
			}
		}
		else
		{
			assert(frame_idx >= curr_f->GetTime() && frame_idx < next_f->GetTime());
			float process = (float) (frame_idx - curr_f->GetTime()) / (next_f->GetTime() - curr_f->GetTime());
			curr_f->GetTweenSprite(curr_f, next_f, sprites, process);
		}
	}
}

}