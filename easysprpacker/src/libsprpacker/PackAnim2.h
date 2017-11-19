#ifndef _EASYSPRPACKER_PACK_ANIM2_H_
#define _EASYSPRPACKER_PACK_ANIM2_H_

#include "PackNode.h"

#include <SM_Vector.h>
#include <rigging.h>
#include <cu/cu_stl.h>

#include <vector>
#include <memory>

namespace libanim2 { class Symbol; }

struct rg_pose_srt;
struct rg_skeleton;
struct rg_timeline;

namespace esprpacker
{

class PackAnim2 : public PackNode
{
public:
	PackAnim2(const std::shared_ptr<libanim2::Symbol>& sym);

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
	void Init(const std::shared_ptr<libanim2::Symbol>& sym);
	void InitSkeleton(const rg_skeleton* sk);
	void InitTimeline(const rg_timeline* tl);
	void InitCurves(const rg_animation* anim);
	
private:
	struct Srt
	{
		sm::vec2 trans;
		float    rot;
		sm::vec2 scale;

		void Init(const rg_pose_srt& srt);

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct Joint
	{
		std::string name;

		uint16_t parent;
		uint16_t children_count;

		Srt local;

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct IK
	{
		uint16_t joints[2];
		uint16_t target;
		uint16_t bend_positive;
		float	 length[2];

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct Skin
	{
		const PackNode* node;		
		int type;
		Srt local;

		Skin();
		~Skin();

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct Slot
	{
		uint16_t joint;
		uint16_t skin;

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct Curve
	{
		float x0, y0;
		float x1, y1;

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct JointSample
	{
		uint16_t time;
		uint8_t	 lerp;
		uint8_t  curve;
		float	 data;

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct TL_Joint
	{
		int type;
		int dims_count[DIM_COUNT];
		std::vector<JointSample> samples;

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct SkinSample
	{
		uint16_t time;
		uint16_t skin;

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct TL_Skin
	{
		std::vector<SkinSample> samples;

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct DeformSample
	{
		uint16_t time;
		uint16_t offset;
		uint8_t  curve;
		uint8_t  padding[3];
		CU_VEC<sm::vec2> data;

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};

	struct TL_Deform
	{
		std::vector<DeformSample> deforms;

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;
	};
	
private:
	// skeleton
	int m_root;
	std::vector<Joint> m_joints;
	std::vector<IK>    m_iks;
	std::vector<Skin>  m_skins;
	std::vector<Slot>  m_slots;

	// animation
	std::vector<TL_Joint>  m_tl_joints;
	std::vector<TL_Skin>   m_tl_skins;
	std::vector<TL_Deform> m_tl_deforms;

	// curves
	std::vector<Curve> m_curves;

}; // PackAnim2

}

#endif // _EASYSPRPACKER_PACK_ANIM2_H_