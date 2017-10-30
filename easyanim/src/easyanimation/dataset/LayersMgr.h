#ifndef _EASYANIM_LAYERS_MGR_H_
#define _EASYANIM_LAYERS_MGR_H_

#include <ee/Observer.h>

namespace eanim
{

class Layer;

class LayersMgr : public ee::Observer
{
public:
	LayersMgr();
	~LayersMgr();

	size_t Size() const {
		return m_layers.size();
	}

	Layer* GetLayer(size_t index) const;
	int QueryIndex(const Layer* layer) const;

	int GetFrameCount() const;

	bool Clear();

//		void removeSprite(const ee::SprPtr& spr);

	const std::vector<Layer*>& GetAllLayers() const {
		return m_layers;
	}

	int GetMaxFrameTime() const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	std::vector<Layer*> m_layers;

}; // LayersMgr

}

#endif // _EASYANIM_LAYERS_MGR_H_