#include "PackUIListTask.h"
#include "PackUI.h"
#include "pack_ui_cfg.h"

#include <drag2d.h>

namespace librespacker
{

PackUIListTask::PackUIListTask(const std::string& filepath,
							   const Json::Value& value)
	: PackUITask(filepath)
{
	std::string wrapper_filepath = value["wrapper filepath"].asString();
	wrapper_filepath = d2d::FilenameTools::getAbsolutePathFromFile(filepath, wrapper_filepath);
	wrapper_filepath = d2d::FilenameTools::FormatFilepathAbsolute(wrapper_filepath);

	PackUI::Instance()->Instance()->AddListener(wrapper_filepath, this);
}

void PackUIListTask::OnKnownPackID(const std::string& filepath, int id)
{
	m_id = id;
}

void PackUIListTask::Output(const std::string& dir, Json::Value& value) const
{
	Json::Value item_val;
	item_val["filepath"] = d2d::FilenameTools::getRelativePath(dir, m_filepath).ToStdString();
	item_val["wrapper id"] = m_id;
	item_val["type"] = UI_LIST;
	value[value.size()] = item_val;
}

}