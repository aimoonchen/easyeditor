#pragma once

#include <drag2d.h>

#include "LibraryPage.h"

namespace complex
{
	class LibraryPage;

	class LibraryPanel : public d2d::LibraryPanel
	{
	public:
		LibraryPanel(wxWindow* parent);

		//
		//ISerializable interface
		//

		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		d2d::ILibraryPage* getImagePage() { return m_imagePage; }
		d2d::ILibraryPage* getComplexPage() { return m_complexPage; }
		d2d::ILibraryPage* getAnimPage() { return m_animPage; }
		d2d::ILibraryPage* get9PatchPage() { return m_9patchPage; }
		d2d::ILibraryPage* getFontPage() { return m_fontPage; }
		d2d::ILibraryPage* getScriptsPage() { return m_scriptsPage; }

	private:
		d2d::LibraryImagePage* m_imagePage;
		LibraryPage* m_complexPage;
		d2d::LibraryAnimPage* m_animPage;
		d2d::Library9PatchPage* m_9patchPage;
		d2d::LibraryFontBlankPage* m_fontPage;
		d2d::LibraryScriptsPage* m_scriptsPage;

	}; // LibraryPanel
}

