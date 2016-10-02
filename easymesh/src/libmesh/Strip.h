#ifndef _EASYMESH_STRIP_H_
#define _EASYMESH_STRIP_H_

#include "Mesh.h"

#include <sprite2/StripMesh.h>

namespace emesh
{

class Strip : public Mesh, public s2::StripMesh
{
public:
	Strip();
	Strip(const Strip& strip);
	Strip(const ee::Symbol* base);

// 	/**
// 	 *  @interface
// 	 *    Cloneable
// 	 */
// 	virtual Strip* Clone() const;

	/**
	 *  @interface
	 *    Mesh
	 */
	virtual void Load(const Json::Value& value);
	virtual void Store(Json::Value& value) const;
	virtual void OffsetUV(float dx, float dy);

	/**
	 *  @interface
	 *    Editable
	 */
	virtual void TraverseMesh(ee::Visitor<ee::Shape>& visitor) const {}
	virtual bool RemoveMesh(ee::Shape* shape) { return false; }
	virtual bool InsertMesh(ee::Shape* shape) { return false; }
	virtual bool ClearMesh() { return false; }
	virtual void Reset();
	virtual void Clear();

	void InsertNode(const sm::vec2& p);
	void RemoveNode(const sm::vec2& p);
	sm::vec2* FindNode(const sm::vec2& p);
	void MoveNode(sm::vec2* src, const sm::vec2& dst);

	static std::string GetTypeName() { return "strip"; }

private:
//	void InitBound();
	
// 	void RefreshTriangles();
// 	void CopyTriangles(const Strip& strip);

	void AbsorbNodeToRegion(sm::vec2& node);
	void RemoveCornerFromNodes();

	void GetTransList(std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list) const;

	static void TranslateNode(s2::MeshNode* node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list);
	static void TranslateNode(sm::vec2& node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list);

	static void MapUV2XY(const std::vector<sm::vec2>& nodes, int index, const sm::vec2& pos, 
		std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list);

private:
	float m_uv_offset;

}; // Strip 

}

#endif // _EASYMESH_STRIP_H_