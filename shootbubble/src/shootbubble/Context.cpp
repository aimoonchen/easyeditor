#include "Context.h"

#include <stdio.h>

using namespace shootbubble;

Context* Context::m_instance = NULL;

Context::Context() 
{
	property = NULL;
	library = NULL;
	stage = NULL;
	toolbar = NULL;

	row = col = 0;
	edge = 0;
}

Context* Context::Instance()
{
	if (!m_instance)
	{
		m_instance = new Context();
	}
	return m_instance;
}
