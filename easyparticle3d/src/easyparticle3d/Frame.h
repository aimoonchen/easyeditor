#ifndef _EASYPARTICLE3D_FRAME_H_
#define _EASYPARTICLE3D_FRAME_H_



namespace eparticle3d
{

class Task;

class Frame : public ee::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

protected:
	virtual void OnSaveAs(wxCommandEvent& event);

private:
	void SaveAsParticle3d(const wxString& filepath) const;
	void SaveAsAnim(const wxString& filepath) const;
	void SaveAsInvert(const wxString& filepath) const;

}; // Frame

}

#endif // _EASYPARTICLE3D_FRAME_H_
