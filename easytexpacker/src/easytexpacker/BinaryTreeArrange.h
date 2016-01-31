#ifndef _EASYTEXPACKER_BINARY_TREE_ARRANGE_H_
#define _EASYTEXPACKER_BINARY_TREE_ARRANGE_H_

#include "BaseStrategy.h"

namespace etexpacker
{
	class BinaryTreeArrange : public BaseStrategy
	{
	public:
		BinaryTreeArrange();
		virtual ~BinaryTreeArrange();

		virtual void arrange(const std::vector<ee::ImageSprite*>& sprites);
		virtual int GetTextureAccount() const { return 1; }

	private:
		void resetRoot();

	private:
		struct Node
		{
			Node* child[2];
			ee::Sprite* texture;
			ee::Rect rc;

			Node();
			~Node();

			Node* insert(ee::ImageSprite* sprite, int flag);
		};

		static const int REINSERT_MAX = 128;
		static const int SPACING = 128;

	private:
		Node* m_root;

	}; // BinaryTreeArrange
}

#endif // _EASYTEXPACKER_BINARY_TREE_ARRANGE_H_