#ifndef _EASYEDITOR_LIBRARY_PAGE_H_
#define _EASYEDITOR_LIBRARY_PAGE_H_

#include "Visitor.h"
#include "Symbol.h"

#include <string>

#include <wx/wx.h>

namespace ee
{

class ListItem;
class Symbol;
class LibraryList;

class LibraryPage : public wxWindow
{
public:
	LibraryPage(wxWindow* parent, const std::string& name, bool isStatic = false);

	virtual bool IsHandleSymbol(const SymPtr& sym) const = 0;

	virtual void Clear();

	virtual bool LoadFromConfig() { return false; }
	virtual void LoadDefaultSymbol() {}

	virtual void OnActive() {}

	void Traverse(RefVisitor<ListItem>& visitor) const;

	const std::string& GetPageName() const { return m_name; }

	ListItemPtr GetItem(int index = -1) const;
	SymPtr GetSymbol(int index = -1) const;

	void ReloadTexture() const;

	LibraryList* GetList() { return m_list; }

	void AddItem(const ListItemPtr& item);

protected:
	virtual void InitLayout(bool draggable = true);
	virtual void InitLayoutExtend(wxSizer* sizer) {}

	virtual void OnAddPress(wxCommandEvent& event);
	virtual void OnDelPress(wxCommandEvent& event);

protected:
	void InitButtons(wxSizer* sizer);

	bool LoadFromConfig(const std::string& key);

	void OnAddPress(const std::string& type);

protected:
	std::string m_name;

	bool m_is_static;

	wxButton *m_btn_add, *m_btn_del;

	LibraryList* m_list;

}; // LibraryPage

}

#endif // _EASYEDITOR_LIBRARY_PAGE_H_