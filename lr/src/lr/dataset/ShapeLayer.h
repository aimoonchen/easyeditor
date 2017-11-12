#ifndef _LR_SHAPE_LAYER_H_
#define _LR_SHAPE_LAYER_H_

#include "Layer.h"

namespace lr
{

class ShapeLayer : public Layer
{
public:
	ShapeLayer(int id, LibraryPanel* library);

	virtual bool InsertSprite(const ee::SprPtr& spr) override;

}; // ShapeLayer

}

#endif // _LR_SHAPE_LAYER_H_