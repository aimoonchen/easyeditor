#ifndef COMPLEX_SYMBOL_H
#define COMPLEX_SYMBOL_H



namespace ecomplex
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// Object interface
	//
	virtual void Retain() const;
	virtual void Release() const;

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const ee::Matrix& mt, const ee::ColorTrans& color = ee::ColorTrans(), 
		const ee::Sprite* spr = NULL, const ee::Sprite* root = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	bool isOneLayer() const;

	void InitBounding();

	void Clear();

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	static const float SCALE;

public:
	struct Group
	{
		std::string name;
		std::vector<ee::Sprite*> members;
	};

	// todo: 
public:
	std::vector<ee::Sprite*> m_sprites;

	std::vector<Group> m_groups;

	ee::Rect m_rect;

	ee::Rect m_clipbox;
	ee::ShapeStyle m_style;

	bool m_use_render_cache;

private:
	mutable int m_render_version;
	mutable bool m_render_cache_open;

}; // Symbol

}

#endif // COMPLEX_SYMBOL_H