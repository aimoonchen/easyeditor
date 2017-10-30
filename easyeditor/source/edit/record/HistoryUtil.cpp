#include "HistoryUtil.h"

namespace ee
{

Json::Value HistoryUtil::StoreSpritesIndex(const std::vector<SprPtr>& myself,
										   const std::vector<SprPtr>& other)
{
	Json::Value ret;
	int count = 0;
	for (size_t i = 0, n = myself.size(); i < n; ++i)
	{
		int ptr = -1;
		for (size_t j = 0, m = other.size(); j < m; ++j) {
			if (other[j] == myself[i]) {
				ptr = j;
				break;
			}
		}
		if (ptr != -1) {
			ret[count++] = ptr;
		}
	}
	return ret;
}

}