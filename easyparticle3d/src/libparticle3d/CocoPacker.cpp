#include "CocoPacker.h"


#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace eparticle3d
{

void CocoPacker::PackDir(const wxString& srcdir, const wxString& dstfilename)
{
	ebuilder::CodeGenerator gen;

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(srcdir.ToStdString(), files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (ee::FileType::IsType(filepath, ee::FileType::e_particle3d)) {
			PackSingle(filepath, gen);
		}
	}

	std::locale::global(std::locale(""));
	std::ofstream fout(dstfilename.fn_str());
	std::locale::global(std::locale("C"));
	if (!fout.fail())
	{
		fout << gen.toText();
		fout.close();
	}
}

void CocoPacker::PackSingle(const wxString& filepath, ebuilder::CodeGenerator& gen)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	int version = val["version"].asInt();
	if (version == 0) {
		PackOldVersion(val, gen, dir);
	} else {
		PackNewVersion(val, gen, dir);
	}
}

void CocoPacker::PackOldVersion(const Json::Value& val, ebuilder::CodeGenerator& gen,
								const std::string& dir)
{
	std::string s = val["name"].asString();
	lua::TableAssign ta(gen, "['"+s+"']", true);

	lua::assign(gen, "['name']", "'"+s+"',");

	s = val["package"].asString();
	s = "misc";
	lua::assign(gen, "['package']", "'"+s+"',");

	s = wxString::FromDouble(val["count"].asInt());
	lua::assign(gen, "['count']", s+",");
	s = wxString::FromDouble(val["layer"].asInt());
	lua::assign(gen, "['layer']", s+",");

	s = wxString::FromDouble(val["emission_time"].asInt());
	lua::assign(gen, "['emission_time']", s+"*l,");

	s = wxString::FromDouble(val["min_life"].asInt());
	lua::assign(gen, "['min_life']", s+"*l,");

	s = wxString::FromDouble(val["max_life"].asInt());
	lua::assign(gen, "['max_life']", s+"*l,");

	s = wxString::FromDouble(val["min_hori"].asInt());
	lua::assign(gen, "['min_hori']", s+",");

	s = wxString::FromDouble(val["max_hori"].asInt());
	lua::assign(gen, "['max_hori']", s+",");

	s = wxString::FromDouble(val["min_vert"].asInt());
	lua::assign(gen, "['min_vert']", s+",");

	s = wxString::FromDouble(val["max_vert"].asInt());
	lua::assign(gen, "['max_vert']", s+",");

	const float SPEED_SCALE = 0.25f;

	s = wxString::FromDouble(val["min_spd"].asInt() * SPEED_SCALE);
	lua::assign(gen, "['min_spd']", s+"*k,");

	s = wxString::FromDouble(val["max_spd"].asInt() * SPEED_SCALE);
	lua::assign(gen, "['max_spd']", s+"*k,");

	s = wxString::FromDouble(val["gravity"].asInt());
	lua::assign(gen, "['gravity']", s+"*g,");

	s = wxString::FromDouble(val["min_linear_acc"].asInt());
	lua::assign(gen, "['min_linear_acc']", s+",");

	s = wxString::FromDouble(val["max_linear_acc"].asInt());
	lua::assign(gen, "['max_linear_acc']", s+",");

	float center, offset;

	center = val["disturbance_radius"]["center"].asDouble();
	offset = val["disturbance_radius"]["offset"].asDouble();
	s = wxString::FromDouble(center - offset);
	lua::assign(gen, "['min_disturbance_radius']", s+"*k,");
	s = wxString::FromDouble(center + offset);
	lua::assign(gen, "['max_disturbance_radius']", s+"*k,");

	center = val["disturbance_spd"]["center"].asDouble();
	offset = val["disturbance_spd"]["offset"].asDouble();
	s = wxString::FromDouble(center - offset);
	lua::assign(gen, "['min_disturbance_spd']", s+"*k,");
	s = wxString::FromDouble(center + offset);
	lua::assign(gen, "['max_disturbance_spd']", s+"*k,");

	//	s = wxString::FromDouble(val["inertia"].asInt());
	s = wxString::FromDouble(10);
	lua::assign(gen, "['inertia']", s+",");

	s = wxString::FromDouble(val["fadeout_time"].asInt());
	lua::assign(gen, "['fadeout_time']", s+"*l,");

	s = wxString::FromDouble(val["ground"].asInt());
	lua::assign(gen, "['ground']", s+",");

	s = val["additive_blend"].asBool() ? "true" : "false";
	lua::assign(gen, "['additive_blend']", s+",");

	s = wxString::FromDouble(val["start_radius"].asInt());
	lua::assign(gen, "['start_radius']", s+",");

	s = wxString::FromDouble(val["start_height"].asInt());
	lua::assign(gen, "['start_height']", s+",");

	s = val["orient_to_movement"].asBool() ? "true" : "false";
	lua::assign(gen, "['orient_to_movement']", s+",");

	s = val["orient_to_parent"].asBool() ? "true" : "false";
	lua::assign(gen, "['orient_to_parent']", s+",");

	{
		lua::TableAssign ta(gen, "['components']", true);

		int i = 0;
		Json::Value child_val = val["components"][i++];
		while (!child_val.isNull()) {
			lua::TableAssign ta(gen, "", true);

			{
				wxString filepath = ee::FileHelper::GetAbsolutePath(dir, child_val["filepath"].asString());

				Json::Value value;
				Json::Reader reader;
				std::locale::global(std::locale(""));
				std::ifstream fin(filepath.fn_str());
				std::locale::global(std::locale("C"));
				reader.parse(fin, value);
				fin.close();

				s = value["name"].asString();
			}
			//			s = child_val["name"].asString();
			lua::assign(gen, "['name']", "'"+s+"',");

			s = wxString::FromDouble(child_val["start_scale"].asInt());
			lua::assign(gen, "['start_scale']", s+"*s,");

			s = wxString::FromDouble(child_val["end_scale"].asInt());
			lua::assign(gen, "['end_scale']", s+"*s,");

			s = wxString::FromDouble(child_val["min_rotate"].asInt());
			lua::assign(gen, "['min_rotate']", s+"*r,");

			s = wxString::FromDouble(child_val["max_rotate"].asInt());
			lua::assign(gen, "['max_rotate']", s+"*r,");

			PackCompColor(child_val, gen);

			s = wxString::FromDouble(child_val["start_z"].asInt());
			lua::assign(gen, "['start_z']", s+",");

			child_val = val["components"][i++];
		}
	}	
}

void CocoPacker::PackNewVersion(const Json::Value& val, ebuilder::CodeGenerator& gen,
								const std::string& dir)
{
	std::string s = val["name"].asString();
	lua::TableAssign ta(gen, "['"+s+"']", true);

	lua::assign(gen, "['name']", "'"+s+"',");

	s = val["package"].asString();
	s = "misc";
	lua::assign(gen, "['package']", "'"+s+"',");

	s = wxString::FromDouble(val["count"].asDouble());
	lua::assign(gen, "['count']", s+",");
	s = wxString::FromDouble(val["layer"].asInt());
	lua::assign(gen, "['layer']", s+",");

	s = wxString::FromDouble(val["emission_time"].asDouble());
	lua::assign(gen, "['emission_time']", s+"*l,");

	float center = val["life"]["center"].asDouble(),
		offset = val["life"]["offset"].asDouble();

	s = wxString::FromDouble(center - offset);
	lua::assign(gen, "['min_life']", s+"*l,");

	s = wxString::FromDouble(center + offset);
	lua::assign(gen, "['max_life']", s+"*l,");

	s = wxString::FromDouble(val["min_hori"].asInt());
	lua::assign(gen, "['min_hori']", s+",");

	s = wxString::FromDouble(val["max_hori"].asInt());
	lua::assign(gen, "['max_hori']", s+",");

	s = wxString::FromDouble(val["min_vert"].asInt());
	lua::assign(gen, "['min_vert']", s+",");

	s = wxString::FromDouble(val["max_vert"].asInt());
	lua::assign(gen, "['max_vert']", s+",");

	const float SPEED_SCALE = 0.25f;

	center = val["speed"]["center"].asDouble();
	offset = val["speed"]["offset"].asDouble();

	s = wxString::FromDouble((center - offset) * SPEED_SCALE);
	lua::assign(gen, "['min_spd']", s+"*k,");

	s = wxString::FromDouble((center + offset) * SPEED_SCALE);
	lua::assign(gen, "['max_spd']", s+"*k,");

	s = wxString::FromDouble(val["gravity"].asDouble());
	lua::assign(gen, "['gravity']", s+"*g,");

	center = val["linear_acc"]["center"].asDouble();
	offset = val["linear_acc"]["offset"].asDouble();

	s = wxString::FromDouble(center - offset);
	lua::assign(gen, "['min_linear_acc']", s+",");

	s = wxString::FromDouble(center + offset);
	lua::assign(gen, "['max_linear_acc']", s+",");

	center = val["disturbance_radius"]["center"].asDouble();
	offset = val["disturbance_radius"]["offset"].asDouble();
	s = wxString::FromDouble(center - offset);
	lua::assign(gen, "['min_disturbance_radius']", s+"*k,");
	s = wxString::FromDouble(center + offset);
	lua::assign(gen, "['max_disturbance_radius']", s+"*k,");

	center = val["disturbance_spd"]["center"].asDouble();
	offset = val["disturbance_spd"]["offset"].asDouble();
	s = wxString::FromDouble(center - offset);
	lua::assign(gen, "['min_disturbance_spd']", s+"*k,");
	s = wxString::FromDouble(center + offset);
	lua::assign(gen, "['max_disturbance_spd']", s+"*k,");

	//	s = wxString::FromDouble(val["inertia"].asInt());
	s = wxString::FromDouble(10);
	lua::assign(gen, "['inertia']", s+",");

	s = wxString::FromDouble(val["fadeout_time"].asDouble());
	lua::assign(gen, "['fadeout_time']", s+"*l,");

	s = wxString::FromDouble(val["ground"].asInt());
	lua::assign(gen, "['ground']", s+",");

	s = val["additive_blend"].asBool() ? "true" : "false";
	lua::assign(gen, "['additive_blend']", s+",");

	s = wxString::FromDouble(val["start_radius"].asInt());
	lua::assign(gen, "['start_radius']", s+",");

	s = wxString::FromDouble(val["start_height"].asInt());
	lua::assign(gen, "['start_height']", s+",");

	s = val["orient_to_movement"].asBool() ? "true" : "false";
	lua::assign(gen, "['orient_to_movement']", s+",");

	s = val["orient_to_parent"].asBool() ? "true" : "false";
	lua::assign(gen, "['orient_to_parent']", s+",");

	{
		lua::TableAssign ta(gen, "['components']", true);

		int i = 0;
		Json::Value child_val = val["components"][i++];
		while (!child_val.isNull()) {
			lua::TableAssign ta(gen, "", true);

			{
				wxString filepath = ee::FileHelper::GetAbsolutePath(dir, child_val["filepath"].asString());

				Json::Value value;
				Json::Reader reader;
				std::locale::global(std::locale(""));
				std::ifstream fin(filepath.fn_str());
				std::locale::global(std::locale("C"));
				reader.parse(fin, value);
				fin.close();

				s = value["name"].asString();
			}
			//			s = child_val["name"].asString();
			lua::assign(gen, "['name']", "'"+s+"',");

			s = wxString::FromDouble(child_val["scale"]["start"].asDouble());
			lua::assign(gen, "['start_scale']", s+"*s,");

			s = wxString::FromDouble(child_val["scale"]["end"].asDouble());
			lua::assign(gen, "['end_scale']", s+"*s,");

			PackCompColor(child_val, gen);

			float min = child_val["rotate"]["min"].asDouble(),
				max = child_val["rotate"]["max"].asDouble();

			s = wxString::FromDouble(min);
			lua::assign(gen, "['min_rotate']", s+"*r,");

			s = wxString::FromDouble(max);
			lua::assign(gen, "['max_rotate']", s+"*r,");

			s = wxString::FromDouble(child_val["start_z"].asInt());
			lua::assign(gen, "['start_z']", s+",");

			child_val = val["components"][i++];
		}
	}	
}

void CocoPacker::PackCompColor(Json::Value& child_val, ebuilder::CodeGenerator& gen)
{
	std::string s;

	if (child_val["mul_col"].isNull()) {
		lua::assign(gen, "['mul_col_r']", "1,");
		lua::assign(gen, "['mul_col_g']", "1,");
		lua::assign(gen, "['mul_col_b']", "1,");
	} else {
		s = wxString::FromDouble(child_val["mul_col"]["r"].asDouble());
		lua::assign(gen, "['mul_col_r']", s+",");
		s = wxString::FromDouble(child_val["mul_col"]["g"].asDouble());
		lua::assign(gen, "['mul_col_g']", s+",");
		s = wxString::FromDouble(child_val["mul_col"]["b"].asDouble());
		lua::assign(gen, "['mul_col_b']", s+",");
	}

	if (child_val["add_col"].isNull()) {
		lua::assign(gen, "['add_col_r']", "0,");
		lua::assign(gen, "['add_col_g']", "0,");
		lua::assign(gen, "['add_col_b']", "0,");
	} else {
		s = wxString::FromDouble(child_val["add_col"]["r"].asDouble());
		lua::assign(gen, "['add_col_r']", s+",");
		s = wxString::FromDouble(child_val["add_col"]["g"].asDouble());
		lua::assign(gen, "['add_col_g']", s+",");
		s = wxString::FromDouble(child_val["add_col"]["b"].asDouble());
		lua::assign(gen, "['add_col_b']", s+",");
	}

	if (child_val["alpha"].isNull()) {
		lua::assign(gen, "['start_alpha']", "100,");
		lua::assign(gen, "['end_alpha']", "100,");
	} else {
		s = wxString::FromDouble(child_val["alpha"]["start"].asDouble());
		lua::assign(gen, "['start_alpha']", s+",");

		s = wxString::FromDouble(child_val["alpha"]["end"].asDouble());
		lua::assign(gen, "['end_alpha']", s+",");
	}
}

}