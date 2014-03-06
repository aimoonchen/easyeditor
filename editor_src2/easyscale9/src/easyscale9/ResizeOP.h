#pragma once

#include <easyscale9.h>

namespace escale9
{
	class ResizeOP : public escale9::ResizeBaseOP
	{
	public:
		ResizeOP(d2d::EditPanel* editPanel);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onActive();

		virtual bool onDraw() const;

	}; // ResizeOP
}

