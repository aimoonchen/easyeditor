#ifndef _DRAG2D_SUBJECT_ID_H_
#define _DRAG2D_SUBJECT_ID_H_

namespace d2d
{

enum SUBJECT_ID {	
	MSG_SPRITE_NAME_CHANGE = 0,
	MSG_CLEAR_PANEL,
	MSG_REFRESH_PANEL,

	MSG_SELECT_SPRITE,
	MSG_SELECT_SPRITE_SET,
	MSG_REORDER_SPRITE,
	MSG_REORDER_SPRITE_MOST,
	MSG_INSERT_SPRITE,
	MSG_REMOVE_SPRITE,
	MSG_CLEAR_SPRITE,

	MSG_SELECT_SHAPE,
	MSG_SELECT_SHAPE_SET,
	MSG_REMOVE_SHAPE,
	MSG_INSERT_SHAPE,
	MSG_CLEAR_SHAPE,

	MSG_QUERY_SPR_LAYER,

	MSG_SET_CANVAS_DIRTY,

	// layer panel
	MSG_CHANGE_LAYER_MGR_MSG,

	// stage panel
	MSG_EDIT_UNDO,
	MSG_EDIT_REDO,
	MSG_EDIT_ADD_RECORD,
	MSG_GET_KEY_STATE,
	MSG_SET_WND_DIRTY,
};

}

#endif // _DRAG2D_SUBJECT_ID_H_