#include "IconType.h"

namespace eicon
{

IconType get_icon_type(const std::string& desc)
{
	if (desc == "rect") {
		return IT_RECT;
	} else if (desc == "quad") {
		return IT_QUAD;
	} else if (desc == "changed-rect") {
		return IT_CHANGED_RECT;
	} else if (desc == "changed-sector") {
		return IT_CHANGED_SECTOR;
	} else {
		return IT_UNKNOWN;
	}
}

}