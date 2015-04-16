#include "GLCanvas.h"
#include "Camera.h"

#include "view/EditPanel.h"
#include "render/RenderList.h"
#include "render/RenderContext.h"
#include "render/ShaderMgrBase.h"
#include "render/ShaderMgr.h"

#include <wx/wx.h>
#include <gl/glu.h>

namespace d2d
{

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
	EVT_SIZE(GLCanvas::onSize)
	EVT_PAINT(GLCanvas::onPaint)
	EVT_ERASE_BACKGROUND(GLCanvas::onEraseBackground)
 	EVT_MOUSE_EVENTS(GLCanvas::onMouse)
 	EVT_KEY_DOWN(GLCanvas::onKeyDown)
	EVT_KEY_UP(GLCanvas::onKeyUp)
END_EVENT_TABLE()

int gl_attrib[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

GLCanvas::GLCanvas(EditPanel* editPanel)
	: wxGLCanvas(editPanel, wxID_ANY, gl_attrib)
	, m_editPanel(editPanel)
 	, m_camera(editPanel->getCamera())
	, m_screen(editPanel->getCamera())
 	, m_screen_width(0), m_screen_height(0)
 	, m_isInit(false)
 	, m_context(new wxGLContext(this))
{
	m_bgColor.set(1.0f, 1.0f, 1.0f, 1.0f);
}

GLCanvas::~GLCanvas()
{
	delete m_context;
}

void GLCanvas::resetInitState() 
{ 
	m_isInit = false; 
}

void GLCanvas::resetViewport()
{
//	wxLogDebug(_T("GLCanvas::resetViewport()"));
	onSize(wxSizeEvent(m_parent->GetSize()));
}

void GLCanvas::setBgColor(const Colorf& color)
{
	m_bgColor = color;
}

void GLCanvas::SetCurrentCanvas()
{
	SetCurrent(*m_context);
}

void GLCanvas::initGL()
{
	wxLogDebug(_T("GLCanvas::initGL()"));

	ShaderMgr::Instance()->null();

	resetViewport();

	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	if (RenderContext::SHADER_MGR) {
		RenderContext::SHADER_MGR->reload();
	}
}

void GLCanvas::onSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
 	m_screen_width = size.GetWidth();
 	m_screen_height = size.GetHeight();
	onSize(m_screen_width, m_screen_height);
}

void GLCanvas::onPaint(wxPaintEvent& event)
{
	// Makes the OpenGL state that is represented by the OpenGL rendering 
	// context context current
	SetCurrentCanvas();

	if (!m_isInit)
	{
		initGL();
		m_isInit = true;
	}

	glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	onDraw();

	if (RenderContext::SHADER_MGR) {
		RenderContext::SHADER_MGR->Flush();
	}

	glFlush();
	SwapBuffers();

//	wxPaintDC dc(this);
	OnDrawDC();
}

void GLCanvas::onEraseBackground(wxEraseEvent& event)
{	
}

void GLCanvas::onMouse(wxMouseEvent& event)
{
	m_editPanel->onMouse(event);

	// The handler of this event should normally call event.Skip() to allow the default processing 
	// to take place as otherwise the window under mouse wouldn't get the focus.
	if (event.LeftDown()) {
		event.Skip();
	}
}

void GLCanvas::onKeyDown(wxKeyEvent& event)
{
	m_editPanel->onKeyDown(event);
}

void GLCanvas::onKeyUp(wxKeyEvent& event)
{
	m_editPanel->onKeyUp(event);
}

} // d2d