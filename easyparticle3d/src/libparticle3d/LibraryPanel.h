#ifndef _EASYPARTICLE3D_LIBRARY_PANEL_H_
#define _EASYPARTICLE3D_LIBRARY_PANEL_H_

#include <drag2d.h>

namespace eanim { class LibraryPage; }
namespace ecomplex { class LibraryPage; }
namespace escale9 { class LibraryPage; }

namespace eparticle3d
{

class LibraryPanel : public d2d::LibraryPanel
{
public:
	LibraryPanel(wxWindow* parent);

}; // LibraryPanel

}

#endif // _EASYPARTICLE3D_LIBRARY_PANEL_H_