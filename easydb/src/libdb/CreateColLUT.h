#ifndef _EASYDB_CREATE_COL_LUT_H_
#define _EASYDB_CREATE_COL_LUT_H_

#include "ICommand.h"

#include <stdint.h>

namespace ee { class CreateColLUT; }

namespace edb
{

class CreateColLUT : public ICommand
{
public:
	CreateColLUT() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new CreateColLUT(); }

private:
	void Run(const std::string& filepath) const;

	static void DrawPixel(uint8_t* pixels, int grid, int w, int h, int x, int y, int r, int g, int b);
	static void PixelTo256(uint8_t* dst, int grid, int r, int g, int b);
	static uint8_t Int16To256(int i16, int grid);

}; // CreateColLUT

}

#endif // _EASYDB_CREATE_COL_LUT_H_