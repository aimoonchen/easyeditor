//#ifndef _EASYSCALE9_SCALE9_DATA_H_
//#define _EASYSCALE9_SCALE9_DATA_H_
//
//#include <SM_Vector.h>
//
//#include <json/json.h>
//
//#include <string>
//#include <vector>
//
//namespace ee { class Sprite; class RenderParams; }
//namespace s2 { class RenderParams; class Sprite; }
//
//namespace escale9
//{
//
//enum Scale9Type
//{
//	e_null = 0,
//	e_9Grid,
//	e_3GridHor,
//	e_3GridVer,
//	e_6GridUpper,
//	e_9GridHollow
//};
//
//class Scale9Data
//{
//public:
//	Scale9Data();
//	Scale9Data(const Scale9Data& data);
//	~Scale9Data();
//
//	void ReloadTexture() const;
//
//	void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
//
//	void GetSize(float& width, float& height) const;
//
//	void Resize(float width, float height);
//	void Compose(ee::Sprite* sprs[3][3], float width, float height);
//
//	void LoadFromFile(const std::string& filepath);
//
//	Scale9Type GetType() const { return m_type; }
//	
//	ee::Sprite* GetSprite(int i, int j) const { return m_sprs[i][j]; }
//
//	static void DrawScale9(Scale9Type type, ee::Sprite* const sprs[3][3], 
//		const s2::RenderParams& params);
//
//	static void ResizeScale9(Scale9Type type, ee::Sprite* const sprs[3][3],
//		float width, float height);
//	static void ResizeSprite(ee::Sprite* spr, const sm::vec2& center, 
//		float width, float height);
//
//	static Scale9Type CheckType(ee::Sprite* sprs[3][3]);
//
//private:
//	static void InitSprite(const Json::Value& spr_val,
//		ee::Sprite** pSprite, const std::string& dir);	
//
//private:
//	Scale9Type m_type;
//
//	// 2
//	// 1
//	// 0 1 2
//	ee::Sprite* m_sprs[3][3];
//
//	float m_width, m_height;
//
//}; // Scale9Data
//
//}
//
//#endif // _EASYSCALE9_SCALE9_DATA_H_