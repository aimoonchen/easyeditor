#ifndef _EASYEDITOR_SPRITE_CTRL_NODE_H_
#define _EASYEDITOR_SPRITE_CTRL_NODE_H_

#include "Vector.h"

namespace ee
{

class Sprite;

class SpriteCtrlNode
{
public:
	static void GetSpriteCtrlNodes(const Sprite* sprite, Vector nodes[8]);

	static void GetSpriteCtrlNodesExt(const Sprite* sprite, Vector nodes[4]);

public:
 	// 0 4 1
 	// 5   6
 	// 2 7 3
 	enum Type
 	{
 		LEFT_UP = 0,
 		RIGHT_UP,
 		LEFT_DOWN,
 		RIGHT_DOWN,
 		UP,
 		LEFT,
 		RIGHT,
 		DOWN
 	};
 
 	struct Node
 	{
 		Vector pos;
 		Type type;
 
 		void setInvalid() {
 			pos.SetInvalid();
 		}
 		bool isValid() {
 			return pos.IsValid();
 		}
 	};

}; // SpriteCtrlNode

}

#endif // _EASYEDITOR_SPRITE_CTRL_NODE_H_