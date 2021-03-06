#ifndef _EASYTEXPACKER_DIRECTLY_ARRANGE_H_
#define _EASYTEXPACKER_DIRECTLY_ARRANGE_H_

#include "BaseStrategy.h"

namespace etexpacker
{

class DirectlyArrange : public BaseStrategy
{
public:
	virtual void Arrange(const std::vector<ee::ImageSprite*>& sprs) override;
	virtual int GetTextureAccount() const override { return 1; }

}; // DirectlyArrange

}

#endif // _EASYTEXPACKER_DIRECTLY_ARRANGE_H_