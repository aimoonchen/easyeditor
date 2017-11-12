#ifndef _EASYRESPACKER_PACK_PNG_H_
#define _EASYRESPACKER_PACK_PNG_H_

#include "PackImage.h"

#include <stdint.h>

namespace erespacker
{

class PackPNG : public PackImage
{
public:
	PackPNG(bool png8);
	virtual ~PackPNG();

	virtual void Load(const std::string& filepath) override;
	virtual void Store(const std::string& filepath, float scale) const override;

private:
	void Clear();

	void GenPng4();

	void Store(std::ofstream& fout, uint8_t* buffer, 
		int width, int height) const;
 	void StoreScaled(std::ofstream& fout, float scale) const;	

private:
	int m_width, m_height;

	uint8_t* m_buffer;

}; // PackPNG

}

#endif // _EASYRESPACKER_PACK_PNG_H_