#include "RegularRectCut.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolMgr.h>
#include <ee/StringHelper.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageData.h>

#include <easyimage.h>

#include <gimg_typedef.h>
#include <gimg_export.h>
#include <pimg/Cropping.h>
#include <sprite2/SymType.h>

#include <wx/arrstr.h>

namespace edb
{

std::string RegularRectCut::Command() const
{
	return "rect-cut";
}

std::string RegularRectCut::Description() const
{
	return "cut image by rect";
}

std::string RegularRectCut::Usage() const
{
	return Command() + " [dir path] [dst path]";
}

int RegularRectCut::Run(int argc, char *argv[])
{
	// rect-cut e:/test2/1001 e:/test2/1001

	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void RegularRectCut::Trigger(const std::string& src_dir, const std::string& dst_dir)
{
// 	ee::ShaderMgr::Instance()->reload();

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());

		std::cout << i << " / " << n << " : " << filepath << "\n";
		if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE)
		{
			auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);

			auto img = ee::ImageDataMgr::Instance()->GetItem(filepath);		
			eimage::RegularRectCut cut(img->GetPixelData(), img->GetWidth(), img->GetHeight());
			cut.AutoCut();

			std::cout << ee::StringHelper::Format("File: %s, Left: %d, Used: %d", filepath.c_str(), cut.GetLeftArea(), cut.GetUseArea()) << std::endl;

			std::string filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
			filename = filename.substr(0, filename.find_last_of('.'));
			ee::StringHelper::ReplaceAll(filename, "\\", "%");
			
			assert(img->GetFormat() == GPF_RGB || img->GetFormat() == GPF_RGBA8);
			int channels = img->GetFormat() == GPF_RGB ? 3 : 4;
			pimg::Cropping crop(img->GetPixelData(), img->GetWidth(), img->GetHeight(), channels, true);

			const std::vector<eimage::Rect>& result = cut.GetResult();
			for (int i = 0, n = result.size(); i < n; ++i)
			{
				const eimage::Rect& r = result[i];
				const uint8_t* pixels = crop.Crop(r.x, r.y, r.x+r.w, r.y+r.h);

				std::string out_path = ee::StringHelper::Format("%s\\%s#%d#%d#%d#%d#", dst_dir.c_str(), filename.c_str(), r.x, r.y, r.w, r.h) + ".png";
				gimg_export(out_path.c_str(), pixels, r.w, r.h, GPF_RGBA8, true);
				delete[] pixels;
			}

			img->RemoveReference();
		}
	}
}

}