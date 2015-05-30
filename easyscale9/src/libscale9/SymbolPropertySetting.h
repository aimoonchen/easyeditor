#ifndef _EASYSCALE9_SYMBOL_PROPERTY_SETTING_H_
#define _EASYSCALE9_SYMBOL_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace escale9
{

class Symbol;

class SymbolPropertySetting : public d2d::SymbolPropertySetting
{
public:
	SymbolPropertySetting(d2d::EditPanel* editPanel, Symbol* symbol);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	Symbol* m_symbol;

}; // SymbolPropertySetting

}

#endif // _EASYSCALE9_SYMBOL_PROPERTY_SETTING_H_