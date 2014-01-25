#ifndef D2D_SCRIPTS_SYMBOL_H
#define D2D_SCRIPTS_SYMBOL_H

#include "ISymbol.h"

namespace d2d
{
	class ScriptsSymbol : public ISymbol
	{
	public:
		ScriptsSymbol() {}
		virtual ~ScriptsSymbol() {}

		//
		// ICloneable interface
		//
		virtual ScriptsSymbol* clone() const { return NULL; }

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin) {}
		virtual void storeToTextFile(std::ofstream& fout) const {}

		//
		// ISymbol interfaces
		//
		virtual void reloadTexture() const {}
		virtual void draw(const ISprite* sprite = NULL) const {}
		virtual float getWidth(const ISprite* sprite = NULL) const { return 0; }
		virtual float getHeight(const ISprite* sprite = NULL) const { return 0; }

		//
		// ListItem interface
		//
		virtual void refresh() {}

		const std::string getContent() const { return m_data; }

	protected:
		virtual void loadResources();

	private:
		std::string m_data;

	}; // ScriptsSymbol
}

#endif // D2D_SCRIPTS_SYMBOL_H