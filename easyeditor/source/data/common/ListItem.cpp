#include "ListItem.h"
#include "Symbol.h"
#include "Bitmap.h"

#include <sprite2/SymType.h>

namespace ee
{

void ListItem::RefreshThumbnail(const std::string& filepath, bool force)
{
	if (force) {
		if (m_bitmap) {
			m_bitmap->RemoveReference();
		}
	} else {
		if (m_bitmap) {
			return;
		}
	}

	Symbol* sym = dynamic_cast<Symbol*>(this);
	if (!sym || sym->Type() == s2::SYM_IMAGE || sym->Type() == s2::SYM_AUDIO) {
		return;
	}

	m_bitmap = new Bitmap(sym);
}

void ListItem::SetBitmap(Bitmap* bmp)
{
	cu::RefCountObjAssign(m_bitmap, bmp);
}

}