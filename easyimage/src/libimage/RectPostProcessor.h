#ifndef _EASYIMAGE_RECT_MANAGER_H_
#define _EASYIMAGE_RECT_MANAGER_H_

#include <drag2d.h>

#include "Rect.h"

namespace eimage
{

class RectPostProcessor
{
public:
	RectPostProcessor(const std::vector<Rect>& rects, 
		int width, int height, bool* ori_pixels);
	~RectPostProcessor();

	void MoveToNoCover();

	void RemoveUnnecessary();

	bool Merge();

	void Align();

	void Reduce();

	void LoadResult(std::vector<Rect>& rects) const;

private:
	struct Item;
	enum Direction
	{
		e_left,
		e_right,
		e_down,
		e_up
	};

	void LoadPixels(const std::vector<Rect>& rects, bool* ori_pixels);

	void UpdateFreedom(Item* item);
	bool IsNoCoverdSide(Item* item, Direction dir) const;
	
	bool IsAlignBetter(Item* item, Direction dir) const;

	bool MoveItem(Item* item, Direction dir);

	void MergeRect(Item* remove, Item* newone);

//	bool PixelHasData(int x, int y) const;
	bool IsPixelCovered(int x, int y) const;
	bool IsPixelImmoveable(int x, int y) const;

	int GetItemDataSize(Item* item) const;

private:
	struct Item
	{
		Item(const Rect& r) : r(r) {
			to_left = to_right = to_down = to_up = true;
		}

		Rect r;

		bool to_left, to_right, to_down, to_up;

	}; // Item

	class ItemCmp
	{
	public:
		bool operator () (const Item* lhs, const Item* rhs) const {
			return lhs->r.h * lhs->r.w < rhs->r.h * rhs->r.w;
		}
	}; // ItemCmp

	class Pixel
	{
	public:
		Pixel(bool has_data) : m_has_data(has_data) {}

		void Add(Item* item) {
			m_items.insert(item);
		}
		void Remove(Item* item) {
			m_items.erase(item);
		}
		bool Find(Item* item) const {
			return m_items.find(item) != m_items.end();
		}
		bool IsCoverd() const {
			return m_items.size() > 1;
		}
		bool IsEmpty() const {
			return m_items.empty();
		}

		bool HasData() const {
			return m_has_data;
		}

		Item* FindSpecialRect(int x, int y, int w, int h) const;

	private:
		std::set<Item*> m_items;

		bool m_has_data;

	}; // Pixel

private:
	int m_width, m_height;

	std::multiset<Item*, ItemCmp> m_items;
	Pixel** m_pixels;

}; // RectManager

}

#endif // _EASYIMAGE_RECT_MANAGER_H_