#ifndef _SPRITE_IMPORT_NODE_SCALE9_H_
#define _SPRITE_IMPORT_NODE_SCALE9_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;

class NodeScale9 : private cu::Uncopyable
{
public:
	struct Grid
	{
		uint32_t sym;
		uint16_t dir;
		uint16_t mirror;
	};

	uint32_t type;
	Grid grids[1];

public:
	NodeScale9(Allocator& alloc, ImportStream& is);
	
	static int Size();

}; // NodeScale9

}

#endif // _SPRITE_IMPORT_NODE_SCALE9_H_	