#ifndef _LIBRESPACKER_PICTURE_FROM_BIN_H_
#define _LIBRESPACKER_PICTURE_FROM_BIN_H_

#include "PackPicture.h"

namespace librespacker
{

class PictureFromBin
{
public:
	static int Size(const PackPicture* pic);

	static void Unpack(uint8_t** ptr, const std::vector<d2d::Image*>& images,
		PackPicture* pic);

private:
	static void UnpackQuad(uint8_t** ptr, const std::vector<d2d::Image*>& images,
		PackPicture::Quad& quad);

}; // PictureFromBin

}

#endif // _LIBRESPACKER_PICTURE_FROM_BIN_H_