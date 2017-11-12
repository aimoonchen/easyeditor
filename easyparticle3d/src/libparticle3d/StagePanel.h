#ifndef _EASYPARTICLE3D_STAGEPANEL_H_
#define _EASYPARTICLE3D_STAGEPANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>

#include <SM_Matrix.h>

namespace eparticle3d
{

class LibraryPanel;
class ParticleSystem;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		LibraryPanel* library);
	virtual ~StagePanel();

	virtual bool UpdateStage() override;

	void SetPSMat(const sm::vec2& pos);
	const S2_MAT& GetPSMat() const { return m_ps_mat; }

private:
	void UpdateControl();

public:
	ParticleSystem* m_ps;

	float xRot, yRot;

private:
	S2_MAT m_ps_mat;

}; // StagePanel

}

#endif // _EASYPARTICLE3D_STAGEPANEL_H_
