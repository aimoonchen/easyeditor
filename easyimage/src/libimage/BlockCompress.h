#ifndef _EASYIMAGE_BLOCK_COMPRESS_H_
#define _EASYIMAGE_BLOCK_COMPRESS_H_

#include <string>
#include <cu/cu_stl.h>
#include <stdint.h>

namespace eimage
{

class BlockCompress
{
public:
	BlockCompress(const CU_VEC<std::string>& image_files);
	~BlockCompress();

	void Compress();
	void Uncompress(const std::string& dir) const;

	void PrintStatistics() const;

private:
	struct Picture;

	class Block
	{
	public:
		Block(uint8_t* pixels, int w, int h, int x, int y);
		~Block();

		bool IsSame(const Block* b) const;

		void CopyToPicture(const Picture& pic, int x, int y, uint8_t* pixels) const;

	private:
		int PixelDistanceSquare(const Block* b, int x, int y) const;

	private:
		uint8_t* m_pixels;

	}; // Block

	struct Picture
	{
		std::string filepath;
		int w, h;
		CU_VEC<Block*> blocks;
	}; // Picture

private:
	void Compress(const std::string& filepath);
	void Uncompress(const std::string& dir, const Picture& pic) const;

	static bool IsBlockTransparent(uint8_t* pixels, int w, int h, int x, int y);

private:
	static const int TOLERANCE;

private:
	CU_VEC<std::string> m_image_files;

	CU_VEC<Block*> m_blocks;

	CU_VEC<Picture*> m_debug_pic;	

	// statistics
	int m_tot_area;
	int m_tot_block, m_used_block, m_saved_block;

}; // BlockCompress

}

#endif // _EASYIMAGE_BLOCK_COMPRESS_H_