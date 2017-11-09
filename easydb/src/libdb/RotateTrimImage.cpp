#include "RotateTrimImage.h"
#include "check_params.h"
#include "utility.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/ImageSymbol.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/MinBoundingBox.h>
#include <ee/SymbolFile.h>
#include <ee/ImageData.h>

#include <easyimage.h>

#include <SM_Calc.h>
#include <sprite2/SymType.h>
#include <sprite2/DrawRT.h>
#include <gum/Config.h>

#include <wx/arrstr.h>

#include <fstream>
#include <iostream>

namespace edb
{

static const char* OUTPUT_FILE = "rotate-trim";

std::string RotateTrimImage::Command() const
{
	return "rotate-trim";
}

std::string RotateTrimImage::Description() const
{
	return "rotate and trim image, clip blank part";
}

std::string RotateTrimImage::Usage() const
{
	return Command() + " [dir path]";
}

int RotateTrimImage::Run(int argc, char *argv[])
{
	// rotate-trim e:/test2/1001

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;

	gum::Config* cfg = gum::Config::Instance();
	bool ori_alpha_cfg = cfg->GetPreMulAlpha();
	cfg->SetPreMulAlpha(false);

	RotateTrim(argv[2]);

	data.open_image_edge_clip = ori_clip_cfg;
	cfg->SetPreMulAlpha(ori_alpha_cfg);

	return 0;
}

const char* RotateTrimImage::GetOutputFileName()
{
	return OUTPUT_FILE;
}

void RotateTrimImage::RotateTrim(const std::string& dir)
{
	std::string output_file = dir + "\\" + OUTPUT_FILE;
	std::locale::global(std::locale(""));	
	std::ofstream fout(output_file.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	
	if (fout.fail()) {
		std::cout << "Can't open output file. \n";
		return;
	}

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		std::cout << i << " / " << n << " : " << filepath << "\n";

		if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE)
		{
			auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);

			auto img_data = ee::ImageDataMgr::Instance()->GetItem(filepath);
			int width, height;
			sm::vec2 center;
			float angle;
			bool success = GetRotateTrimInfo(img_data->GetPixelData(), img_data->GetWidth(), 
				img_data->GetHeight(), width, height, center, angle);
			if (!success || angle == 0) {
				continue;
			} 

			auto spr = ee::SpriteFactory::Instance()->Create(sym);
			spr->SetPosition(center);
			spr->SetAngle(angle);

			s2::DrawRT rt;
			rt.Draw(*spr, true, width, height);
			rt.StoreToFile(filepath.c_str(), width, height);

			//std::string dir = ee::FileHelper::getFileDir(filepath);
			//std::string name = ee::FileHelper::getFilename(filepath);
 		//	std::string outpath = dir + "\\test_" + name + ".png";
 		//	ss.SaveToFile(outpath, width, height);

			// output info
			std::string path = ee::FileHelper::GetRelativePath(dir, filepath);
			fout << path << " " << center.x << " " << center.y << " " << angle << "\n";
		}
	}

	fout.close();
}

bool RotateTrimImage::GetRotateTrimInfo(const uint8_t* pixels, int img_w, int img_h, 
										int& width, int& height, sm::vec2& center, float& angle) const
{
	eimage::ExtractOutlineRaw raw(pixels, img_w, img_h);
	raw.CreateBorderLineAndMerge();
	if (raw.GetBorderLine().empty()) {
		return false;
	}
	raw.CreateBorderConvexHull();

	sm::vec2 bound[4];
	bool is_rotate = ee::MinBoundingBox::Do(raw.GetConvexHull(), bound);

	center = (bound[0] + bound[2]) * 0.5f;
	center.x -= img_w * 0.5f;
	center.y -= img_h * 0.5f;

	center = -center;

	if (is_rotate) {
		float left = FLT_MAX;
		int left_idx;
		for (int i = 0; i < 4; ++i) {
			if (bound[i].x < left) {
				left = bound[i].x;
				left_idx = i;
			}
		}

		const sm::vec2& s = bound[left_idx];
		const sm::vec2& e = bound[left_idx == 3 ? 0 : left_idx + 1];
		sm::vec2 right = s;
		right.x += 1;
		angle = -sm::get_angle(s, e, right);
		center = sm::rotate_vector(center, angle);

		width = std::ceil(sm::dis_pos_to_pos(s, e));
		height = std::ceil(sm::dis_pos_to_pos(e, bound[(left_idx+2)%4]));
	} else {
		angle = 0;
	}

	return true;
}

}