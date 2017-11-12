#ifndef _EASYSPRPACKER_PACK_PARTICLE3D_H_
#define _EASYSPRPACKER_PACK_PARTICLE3D_H_

#include "PackNode.h"

#include <ps_3d.h>
#include <sprite2/Particle3dSymbol.h>

#include <vector>

namespace eparticle3d { class Symbol; }

namespace esprpacker
{

class PackParticle3d : public PackNode
{
public:
	PackParticle3d(const std::shared_ptr<eparticle3d::Symbol>& sym);

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp) const override;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const override;
	virtual int SizeOfPackToBin() const override;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const override;	
	
private:
	void Init(const std::shared_ptr<eparticle3d::Symbol>& sym);	

	void PackToLuaPS(ebuilder::CodeGenerator& gen) const;

private:
	class Component
	{
	public:
		Component(const p3d_symbol& sym);
		~Component();

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;

		int SizeOfUnpackFromBin() const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;

	public:
		int m_count;

		float m_scale_start, m_scale_end;

		float m_angle, m_angle_var;

		uint32_t m_mul_col_begin, m_mul_col_end, 
			     m_add_col_begin, m_add_col_end;

		const PackNode* m_node;

	}; // Component

private:
	int m_blend;

	bool m_static_mode;

	float m_emission_time;
	int m_count;

	float m_life, m_life_var;

	float m_hori, m_hori_var;
	float m_vert, m_vert_var;

	float m_radial_spd, m_radial_spd_var;
	float m_tangential_spd, m_tangential_spd_var;
	float m_angular_spd, m_angular_spd_var;

	float m_dis_region, m_dis_region_var;
	float m_dis_spd, m_dis_spd_var;

	float m_gravity;

	float m_linear_acc, m_linear_acc_var;

	float m_fadeout_time;

	int m_ground;

	float m_start_radius;
	float m_start_height;

	bool m_orient_to_movement;

	bool m_loop;
	bool m_local;

	std::vector<Component> m_components;

}; // PackParticle3d

}

#endif // _EASYSPRPACKER_PACK_PARTICLE3D_H_