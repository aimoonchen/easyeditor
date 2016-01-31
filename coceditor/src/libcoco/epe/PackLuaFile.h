#ifndef _COCO_EPE_PACK_LUA_FILE_H_
#define _COCO_EPE_PACK_LUA_FILE_H_



namespace libcoco
{
namespace epe
{

class PackLuaFile
{
public:
	void pack(const std::vector<const ee::Sprite*>& sprites, 
		const std::string& outfloder);

private:
	

}; // PackLuaFile

}
}

#endif // _COCO_EPE_PACK_LUA_FILE_H_