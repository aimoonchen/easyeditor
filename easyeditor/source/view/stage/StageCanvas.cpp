#include <gl/glew.h>

#include "StageCanvas.h"
#include "Exception.h"
#include "ExceptionDlg.h"
#include "EditPanelImpl.h"
#include "KeysState.h"
#include "subject_id.h"
#include "panel_msg.h"
#include "EE_DTex.h"
#include "EE_GTxt.h"

#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <sprite2/RenderCtxStack.h>
#include <sprite2/SprTimer.h>
#include <model3/RenderCtxStack.h>
#include <gum/ShaderLab.h>
#include <gum/DTex.h>
#include <gum/RenderContext.h>
#include <gum/Sprite2.h>
#include <gum/Audio.h>
#include <gum/Model3.h>

#include <shaderlab/Shape2Shader.h>
#include <shaderlab/Shape3Shader.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/Sprite3Shader.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/MaskShader.h>
#include <shaderlab/Model3Shader.h>

namespace ee
{

BEGIN_EVENT_TABLE(StageCanvas, wxGLCanvas)
	EVT_SIZE(StageCanvas::OnSize)
	EVT_PAINT(StageCanvas::OnPaint)
//	EVT_ERASE_BACKGROUND(StageCanvas::OnEraseBackground)
 	EVT_MOUSE_EVENTS(StageCanvas::OnMouse)
 	EVT_KEY_DOWN(StageCanvas::OnKeyDown)
	EVT_KEY_UP(StageCanvas::OnKeyUp)
	EVT_CHAR(StageCanvas::OnChar)
	EVT_TIMER(TIMER_ID, StageCanvas::OnTimer)
	EVT_KILL_FOCUS(StageCanvas::OnKillFocus)
END_EVENT_TABLE()

static const int GL_ATTRIB[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
								  WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
								  WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

static const float FPS = 30;

StageCanvas::StageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
						 wxGLContext* glctx, bool use_context_stack, bool is_3d)
	: wxGLCanvas(stage_wnd, wxID_ANY, GL_ATTRIB)
	, m_share_context(false)
	, m_use_context_stack(use_context_stack)
	, m_is_3d(is_3d)
	, m_stage(stage)
 	, m_width(0), m_height(0)
	, m_dirty(false)
	, m_cam_dirty(false)
	, m_render_ctx_idx(-1)
	, m_timer(this, TIMER_ID)
	, m_draw(true)
{
	if (glctx) {
		m_share_context = true;
		m_gl_ctx = glctx;
	} else {
		m_share_context = false;
		m_gl_ctx = new wxGLContext(this);
		Init();
	}

	m_bg_color.FromFloat(0.5f, 0.5f, 0.5f);
	m_timer.Start(1000 / FPS);

	RegistSubject(SetCanvasDirtySJ::Instance());

	if (m_use_context_stack) 
	{
		if (m_is_3d) {
			m_render_ctx_idx = m3::RenderCtxStack::Instance()->Push(m3::RenderContext());
		} else {
			m_render_ctx_idx = s2::RenderCtxStack::Instance()->Push(s2::RenderContext());
		}
	}
}

StageCanvas::~StageCanvas()
{
	m_timer.Stop();

	if (!m_share_context) {
		delete m_gl_ctx;
	}

	if (m_use_context_stack) 
	{
		if (m_is_3d)
		{
			m3::RenderCtxStack::Instance()->Pop();
		}
		else
		{
			s2::RenderCtxStack::Instance()->Pop();

			const s2::RenderContext* ctx = s2::RenderCtxStack::Instance()->Top();
			if (ctx) {
				gum::RenderContext::Instance()->OnSize(ctx->GetScreenWidth(), ctx->GetScreenHeight());
			}
		}
	}
}

void StageCanvas::SetCurrentCanvas()
{
	SetCurrent(*m_gl_ctx);
	if (m_is_3d) {
		m3::RenderCtxStack::Instance()->Bind(m_render_ctx_idx);
	} else {
		s2::RenderCtxStack::Instance()->Bind(m_render_ctx_idx);
	}
}

void StageCanvas::SetBgColor(const s2::Color& color)
{
	m_bg_color = color;
}

void StageCanvas::OnNotify(int sj_id, void* ud) 
{
	switch (sj_id)
	{
	case MSG_SET_CANVAS_DIRTY:
		m_dirty = true;
		break;
	}
}

void StageCanvas::Init()
{
	SetCurrentCanvas();

	if (glewInit() != GLEW_OK) {
		exit(1);
	}

	s2::SprTimer::Instance()->Init();
	// prepare 2d
	// todo: move to child, for defferent init (such as 3d ?)

//	gum::ShaderLab::Instance()->Init();
	{
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
		mgr->SetContext(rc);
		
		mgr->CreateShader(sl::SHAPE2,	new sl::Shape2Shader(rc));
		mgr->CreateShader(sl::SHAPE3,	new sl::Shape3Shader(rc));
		mgr->CreateShader(sl::SPRITE2,	new sl::Sprite2Shader(rc));
		mgr->CreateShader(sl::SPRITE3,	new sl::Sprite3Shader(rc));
		mgr->CreateShader(sl::BLEND,	new sl::BlendShader(rc));
		mgr->CreateShader(sl::FILTER,	new sl::FilterShader(rc));
		mgr->CreateShader(sl::MASK,		new sl::MaskShader(rc));
		mgr->CreateShader(sl::MODEL3,	new sl::Model3Shader(rc));
	}

	gum::Model3::Instance();
	gum::Sprite2::Instance()->Init();
	gum::Audio::Instance()->Initialize(nullptr, nullptr);
	DTex::Init();
	GTxt::Init();

	try {
// 		ShaderContext::Reload();
// 		ShaderContext::Reset();

		glEnable(GL_TEXTURE_2D);
	} catch (Exception& e) {
		ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();	
	}
}

void StageCanvas::OnSize(wxSizeEvent& event)
{
	SetCurrentCanvas();

	wxSize size = event.GetSize();
 	m_width = size.GetWidth();
 	m_height = size.GetHeight();
	OnSize(m_width, m_height);

	sl::ShaderMgr::Instance()->GetContext()->ClearTextureCache();
}

void StageCanvas::OnPaint(wxPaintEvent& event)
{
	if (!m_draw) {
		return;
	}

	// Makes the OpenGL state that is represented by the OpenGL rendering 
	// context context current
	SetCurrentCanvas();

	OnDrawWhole();
	m_dirty = false;
	m_cam_dirty = false;

	sl::ShaderMgr::Instance()->FlushShader();

	glFlush();
	SwapBuffers();

	gum::ShaderLab::Instance()->Update(1 / 30.0f);

//	wxPaintDC dc(this);
//	OnDrawDC();
}

// void StageCanvas::OnEraseBackground(wxEraseEvent& event)
// {	
// }

void StageCanvas::OnMouse(wxMouseEvent& event)
{
	m_stage->OnMouse(event);

	// The handler of this event should normally call event.Skip() to allow the default processing 
	// to take place as otherwise the window under mouse wouldn't get the focus.
	if (event.LeftDown()) {
		event.Skip();
	}
}

void StageCanvas::OnKeyDown(wxKeyEvent& event)
{
	m_stage->OnKeyDown(event);
}

void StageCanvas::OnKeyUp(wxKeyEvent& event)
{
	m_stage->OnKeyUp(event);
}

void StageCanvas::OnChar(wxKeyEvent& event)
{
	m_stage->OnChar(event);
}

void StageCanvas::OnTimer(wxTimerEvent& event)
{
	OnTimer();

	float dt = 0.166f;
	clock_t curr_time = clock();
	static clock_t last_time = 0;
	if (last_time != 0) {
		dt = (float)(curr_time - last_time) / CLOCKS_PER_SEC;
	}
	last_time = curr_time;

	if (s2::SprTimer::Instance()->Update(dt)) {
		m_dirty = true;
	}

	gum::DTex::Instance()->Flush();

	bool dirty = m_stage->Update();
	if (dirty) {
		m_dirty = dirty;
	}

	if (m_dirty || m_cam_dirty) {
		Refresh();
	}
}

void StageCanvas::OnKillFocus(wxFocusEvent& event)
{
	m_stage->GetKeyState().Reset();
}

}