#ifndef _EASYTERRAIN2D_OCEAN_MESH_H_
#define _EASYTERRAIN2D_OCEAN_MESH_H_

#include <easyshape.h>

#include <cu/CU_RefCountObj.h>

namespace eterrain2d
{

class MeshShape;

class OceanMesh : public cu::RefCountObj
{
public:
	OceanMesh(const eshape::PolygonShape* shape, const std::shared_ptr<ee::ImageSymbol>& image);
	virtual ~OceanMesh();

	void Build();

	const eshape::PolygonShape* GetBounding() const { return m_shape; }
	const std::shared_ptr<ee::ImageSymbol>& GetImage0() const { return m_image0; }
	const std::shared_ptr<ee::ImageSymbol>& GetImage1() const { return m_image1; }
	bool IsWaveOpen() const { return m_wave_open; }
	int GetWaveGridRow() const { return m_row; }
	int GetWaveGridCol() const { return m_col; }
	float GetWaveSpeed() const { return m_wave_speed; }
	float GetWaveHeight() const { return m_wave_height; }
	bool IsUVMoveOpen() const { return m_uv_move_open; }
	const sm::vec2& GetUVMoveSpeed() const { return m_texcoords_spd; }
	bool IsTexBlendOpen() const { return m_blend_open; }
	float GetTexBlendSpeed() const { return m_blend_speed; }

	void SetSmallGridSize(int row, int col);
	void SetWaveInfo(float speed, float height);
	void SetTexcoordsSpeed(const sm::vec2& speed);
	void SetBlendSpeed(float spd);

	void Update(float dt);
	void Draw(const s2::RenderParams& params, bool draw_tris) const;

	const eshape::PolygonShape* GetShape() const {
		return m_shape;
	}

	void OpenWave(bool open);
	void OpenUVMove(bool open);
	void OpenBlend(bool open);

	bool IsBlendOpen() const { return m_blend_open; }
	void SetImage1(const std::shared_ptr<ee::ImageSymbol>& image);

	sm::rect GetRegion() const;

	void SetBoundLock(bool lock) { m_lock_bound = lock; }
	bool IsBoundLockOpen() const { return m_lock_bound; }

	const std::vector<MeshShape*>& GetMeshes() const { return m_grids; }

	float GetTexcoordSpdAngle() const;
	void Rotate(float angle);

private:
	void Clear();

	sm::rect CalBoundRegion(const std::vector<sm::vec2>& bound) const; 
	void CalSegments(const sm::rect& region, std::vector<sm::vec2>& segs) const;
	void CalTrisTexcords(const sm::rect& region, const std::vector<sm::vec2>& vertices,
		std::vector<sm::vec2>& texcoords) const;
	void BuildGrids(const sm::rect& region, const std::vector<sm::vec2>& vertices, 
		const std::vector<sm::vec2>& texcoords, const std::vector<sm::vec2>& bound);

	void UpdateWave(float during);
	void UpdateUVMove(float dt);
	void UpdateBlend(float dt);

private:
	eshape::PolygonShape* m_shape;
	const std::shared_ptr<ee::ImageSymbol> m_image0, m_image1;

	std::vector<MeshShape*> m_grids;

	bool m_wave_open;
	int m_row, m_col;
	float m_wave_speed, m_wave_height;

	bool m_uv_move_open;
	sm::vec2 m_texcoords_base, m_texcoords_spd;

	bool m_blend_open;
	float m_blend_base, m_blend_speed;

	bool m_lock_bound;

	// debug
	mutable sm::rect debug_r;

	float m_during;

}; // OceanMesh

}

#endif // _EASYTERRAIN2D_OCEAN_MESH_H_