#ifndef _LIBPACKER_NORMAL_PACK_H_
#define _LIBPACKER_NORMAL_PACK_H_

#include <vector>
#include <map>
#include <wx/string.h>

#include "Rect.h"
#include "typedef.h"

namespace libpacker
{

class ImageTrimData;

class NormalPack
{
public:
	NormalPack(const std::vector<std::string>& files, const ImageTrimData* trim_info);

	void Pack(int static_size, int max_size = 4096, int min_size = 0);

	void OutputInfo(const std::string& dir, const std::string& dst_file) const;
	void OutputImage(const std::string& filepath) const;

private:
	void Pack(PACK_STRATEGY strategy, int static_size, int max_size = 4096, int min_size = 0);

	void GetExtrude(const int bound[], int w, int h, int& left, int& right, int& bottom, int& up) const;
	int GetExtrude(int max, int tot, int edge) const;

private:
	const std::vector<std::string>& m_filepaths;

	const ImageTrimData* m_trim_info;

	std::vector<RectSize> m_src_sizes;

	std::vector<Rect> m_dst_pos;

	std::vector<libpacker::RectSize> m_dst_img_sz;

	std::vector<std::vector<int> > m_dst_img_idx;

}; // NormalPack

}

#endif // _LIBPACKER_NORMAL_PACK_H_