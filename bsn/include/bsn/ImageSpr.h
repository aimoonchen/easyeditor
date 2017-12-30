#pragma once

#include "bsn/NodeSpr.h"

#include <string>

namespace bsn
{
	
class ImageSpr : public NodeSpr
{
public:
	//
	// serialization
	//
	virtual size_t GetBinSize() const override;
	virtual void StoreToBin(bs::ExportStream& es) const override;
	virtual void StoreToJson(Json::Value& val) const override;

	//
	// deserialization
	//
	virtual void LoadFromBin(mm::LinearAllocator& alloc, bs::ImportStream& is);
	virtual void LoadFromJson(mm::LinearAllocator& alloc, const Json::Value& val);

}; // NodeImgae

}