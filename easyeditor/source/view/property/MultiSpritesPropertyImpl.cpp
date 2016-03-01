#include "MultiSpritesPropertyImpl.h"
#include "BoundingBox.h"
#include "Sprite.h"
#include "sprite_utility.h"
#include "Symbol.h"
#include "color_config.h"
#include "Math2D.h"
#include "panel_msg.h"
#include "SetSpriteColorAOP.h"
#include "SetSpritePosAOP.h"
#include "TranslateSpriteAOP.h"

#include <wx/string.h>

#include <algorithm>

namespace ee
{

const wxChar* MultiSpritesPropertyImpl::BOOL_3TYPE_LABELS[] 
	= { wxT("[?]"), wxT("True"), wxT("False"), NULL };		

const wxChar* MultiSpritesPropertyImpl::ALIGN_LABELS[] 
	= { wxT("��"), wxT("��"), wxT("��"), wxT("��"), wxT("��"), wxT("������"), wxT("������"), wxT("������"), wxT("������"), NULL };

const wxChar* MultiSpritesPropertyImpl::CENTER_LABELS[] 
	= { wxT("��"), wxT("ˮƽ"), wxT("��ֱ"), wxT("���"), NULL };

MultiSpritesPropertyImpl::MultiSpritesPropertyImpl(const std::vector<Sprite*>& sprites)
{
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Retain();
	}
	m_sprites = sprites;
}

MultiSpritesPropertyImpl::~MultiSpritesPropertyImpl()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
}

std::string MultiSpritesPropertyImpl::GetTag() const
{
	if (m_sprites.empty()) {
		return "";
	}

	std::string tag = m_sprites[0]->tag;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (tag != m_sprites[i]->tag) {
			return "[different]";
		}
	}
	return tag;
}

const wxChar* MultiSpritesPropertyImpl::GetClip() const
{
	if (m_sprites.empty()) {
		return BOOL_3TYPE_LABELS[e_bool_different];
	}

	bool clip = m_sprites[0]->clip;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (clip != m_sprites[i]->clip) {
			return BOOL_3TYPE_LABELS[e_bool_different];
		}
	}
	return clip ? BOOL_3TYPE_LABELS[e_bool_true] : BOOL_3TYPE_LABELS[e_bool_false];
}

Vector MultiSpritesPropertyImpl::GetPosition() const
{
	if (m_sprites.empty()) {
		Vector p;
		p.SetInvalid();
		return p;
	}

	Vector p = m_sprites[0]->GetPosition();
	for (size_t i = 1, n = m_sprites.size(); i < n; ++i)
	{
		if (p != m_sprites[i]->GetPosition())
		{
			p.SetInvalid();
			break;
		}
	}
	return p;
}

Colorf MultiSpritesPropertyImpl::GetMultiColor() const
{
	if (m_sprites.empty()) {
		return WHITE;
	}

	Colorf col = m_sprites[0]->color.multi;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->color.multi) {
			return WHITE;
		}
	}
	return col;
}

Colorf MultiSpritesPropertyImpl::GetAddColor() const
{
	if (m_sprites.empty()) {
		return BLACK;
	}
	
	Colorf col = m_sprites[0]->color.add;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->color.add) {
			return BLACK;
		}
	}
	return col;
}

Colorf MultiSpritesPropertyImpl::GetTransColorR() const
{
	if (m_sprites.empty()) {
		return RED;
	}

	Colorf col = m_sprites[0]->color.r;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->color.r) {
			return RED;
		}
	}
	return col;
}

Colorf MultiSpritesPropertyImpl::GetTransColorG() const
{
	if (m_sprites.empty()) {
		return GREEN;
	}

	Colorf col = m_sprites[0]->color.g;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->color.g) {
			return GREEN;
		}
	}
	return col;
}

Colorf MultiSpritesPropertyImpl::GetTransColorB() const
{
	if (m_sprites.empty()) {
		return BLUE;
	}

	Colorf col = m_sprites[0]->color.b;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->color.b) {
			return BLUE;
		}
	}
	return col;
}

float MultiSpritesPropertyImpl::GetAngle() const
{
	if (m_sprites.empty()) {
		return 0;
	}

	float angle = m_sprites[0]->GetAngle();
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (angle != m_sprites[i]->GetAngle()) {
			return 0;
		}
	}
	return angle;
}

Vector MultiSpritesPropertyImpl::GetScale() const
{
	if (m_sprites.empty()) {
		return Vector(1, 1);
	}

	Vector scale = m_sprites[0]->GetScale();
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (scale != m_sprites[i]->GetScale()) {
			return Vector(1, 1);
		}
	}
	return scale;
}

bool MultiSpritesPropertyImpl::GetMirrorX() const
{
	if (m_sprites.empty()) {
		return false;
	}

	bool mirror = m_sprites[0]->GetMirrorX();
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (mirror != m_sprites[i]->GetMirrorX()) {
			return false;
		}
	}
	return mirror;
}

bool MultiSpritesPropertyImpl::GetMirrorY() const
{
	if (m_sprites.empty()) {
		return false;
	}

	bool mirror = m_sprites[0]->GetMirrorY();
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (mirror != m_sprites[i]->GetMirrorY()) {
			return false;
		}
	}
	return mirror;
}

bool MultiSpritesPropertyImpl::GetVisible() const
{
	if (m_sprites.empty()) {
		return true;
	}

	bool ret = m_sprites[0]->visiable;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (ret != m_sprites[i]->visiable) {
			return true;
		}
	}
	return ret;
}

bool MultiSpritesPropertyImpl::GetEditable() const
{
	if (m_sprites.empty()) {
		return true;
	}

	bool ret = m_sprites[0]->editable;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (ret != m_sprites[i]->editable) {
			return true;
		}
	}
	return ret;
}

void MultiSpritesPropertyImpl::SetTag(const std::string& tag)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->tag = tag;
	}
}

void MultiSpritesPropertyImpl::SetClip(int clip)
{
	SetWndDirtySJ::Instance()->SetDirty();

	Bool3Type type = (Bool3Type)clip;
	if (type == e_bool_true) {
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_sprites[i]->clip = true;
		}
	} else if (type == e_bool_false) {
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_sprites[i]->clip = false;
		}
	}
}

void MultiSpritesPropertyImpl::SetPos(float x, float y)
{
	Vector pos(x, y);
	EditAddRecordSJ::Instance()->Add(new SetSpritePosAOP(m_sprites, pos));
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetTransform(pos, spr->GetAngle());
	}
}

void MultiSpritesPropertyImpl::SetColorMul(const Colorf& col)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		float alpha = m_sprites[i]->color.multi.a;
		m_sprites[i]->color.multi = col;
		m_sprites[i]->color.multi.a = alpha;
	}
}

void MultiSpritesPropertyImpl::SetColorAdd(const Colorf& col)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		float alpha = m_sprites[i]->color.add.a;
		m_sprites[i]->color.add = col;
		m_sprites[i]->color.add.a = alpha;
	}
}

void MultiSpritesPropertyImpl::SetColorAlpha(float alpha)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->color.multi.a = alpha;
	}
}

void MultiSpritesPropertyImpl::SetAngle(bool overall, float angle)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall) 
	{
		Vector center = GetOverallCenter();
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			Vector pos = Math2D::RotateVector(spr->GetPosition() - center, angle);
			spr->SetTransform(pos, angle);
		}
	} 
	else 
	{
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			spr->SetTransform(spr->GetPosition(), angle);
		}
	}
}

void MultiSpritesPropertyImpl::SetScale(bool overall, float sx, float sy)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall)
	{
		Vector center = GetOverallCenter();
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			Vector pos = spr->GetPosition() - center;
			pos.x *= sx;
			pos.y *= sy;
			spr->SetTransform(pos + center, spr->GetAngle());
		}
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->SetScale(Vector(sx, sy));
	}
}

void MultiSpritesPropertyImpl::SetMirrorX(bool overall, bool mirror)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall)
	{
		Vector center = GetOverallCenter();
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			Vector pos = spr->GetPosition() - center;
			pos.x = -pos.x;
			spr->SetTransform(pos + center, -spr->GetAngle());
		}
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetMirror(mirror, spr->GetMirrorY());
	}
}

void MultiSpritesPropertyImpl::SetMirrorY(bool overall, bool mirror)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall)
	{
		Vector center = GetOverallCenter();
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			Vector pos = spr->GetPosition() - center;
			pos.y = -pos.y;
			spr->SetTransform(pos + center, -spr->GetAngle());
		}
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetMirror(spr->GetMirrorX(), mirror);
	}
}

void MultiSpritesPropertyImpl::SetVisible(bool visible)
{
	SetWndDirtySJ::Instance()->SetDirty();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->visiable = visible;
	}
}

void MultiSpritesPropertyImpl::SetEditable(bool editable)
{
	SetWndDirtySJ::Instance()->SetDirty();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->editable = editable;
	}
}

void MultiSpritesPropertyImpl::OnAlign(int align)
{
	SetWndDirtySJ::Instance()->SetDirty();

	AlignType type = (AlignType)align;

	if (type == e_align_left)
	{
		float left = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float l = get_spr_left(sprite);
			if (l < left)
				left = l;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float x = sprite->GetPosition().x + left - get_spr_left(sprite);
			m_sprites[i]->SetTransform(Vector(x, sprite->GetPosition().y), sprite->GetAngle());
		}
	}
	else if (type == e_align_right)
	{
		float right = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float r = get_spr_right(sprite);
			if (r > right)
				right = r;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float x = sprite->GetPosition().x + right - get_spr_right(sprite);
			m_sprites[i]->SetTransform(Vector(x, sprite->GetPosition().y), sprite->GetAngle());
		}
	}
	else if (type == e_align_up)
	{
		float up = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float u = get_spr_up(sprite);
			if (u > up)
				up = u;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float y = sprite->GetPosition().y + up - get_spr_up(sprite);
			m_sprites[i]->SetTransform(Vector(sprite->GetPosition().x, y), sprite->GetAngle());
		}
	}
	else if (type == e_align_down)
	{
		float down = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float d = get_spr_down(sprite);
			if (d < down)
				down = d;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float y = sprite->GetPosition().y + down - get_spr_down(sprite);
			m_sprites[i]->SetTransform(Vector(sprite->GetPosition().x, y), sprite->GetAngle());
		}
	}
	else if (type == e_align_center_left)
	{
		float left = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float x = m_sprites[i]->GetPosition().x;
			if (x < left)
				left = x;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			m_sprites[i]->SetTransform(Vector(left, sprite->GetPosition().y), sprite->GetAngle());
		}
	}
	else if (type == e_align_center_right)
	{
		float right = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float x = m_sprites[i]->GetPosition().x;
			if (x > right)
				right = x;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			m_sprites[i]->SetTransform(Vector(right, sprite->GetPosition().y), sprite->GetAngle());
		}
	}
	else if (type == e_align_center_up)
	{
		float up = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float y = m_sprites[i]->GetPosition().y;
			if (y > up)
				up = y;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			m_sprites[i]->SetTransform(Vector(sprite->GetPosition().x, up), sprite->GetAngle());
		}
	}
	else if (type == e_align_center_down)
	{
		float down = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float y = m_sprites[i]->GetPosition().y;
			if (y < down)
				down = y;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			m_sprites[i]->SetTransform(Vector(sprite->GetPosition().x, down), sprite->GetAngle());
		}
	}
}

void MultiSpritesPropertyImpl::OnCenter(int center)
{
	SetWndDirtySJ::Instance()->SetDirty();

	CenterType type = (CenterType)center;

	if (type == e_center_horizontal)
	{
		float left = FLT_MAX, right = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float x = sprite->GetPosition().x;
			if (x < left) left = x;
			if (x > right) right = x;
		}

		std::vector<Sprite*> sorted(m_sprites);
		std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_x));
		const float space = (right - left) / (sorted.size() - 1);
		for (size_t i = 0, n = sorted.size(); i < n; ++i)
		{
			Sprite* sprite = sorted[i];
			sprite->SetTransform(Vector(left + space * i, sprite->GetPosition().y), sprite->GetAngle());
		}
	}
	else if (type == e_center_vertical)
	{
		float down = FLT_MAX, up = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float y = sprite->GetPosition().y;
			if (y < down) down = y;
			if (y > up) up = y;
		}

		std::vector<Sprite*> sorted(m_sprites);
		std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_y));
		const float space = (up - down) / (sorted.size() - 1);
		for (size_t i = 0, n = sorted.size(); i < n; ++i)
		{
			Sprite* sprite = sorted[i];
			sprite->SetTransform(Vector(sprite->GetPosition().x, down + space * i), sprite->GetAngle());
		}
	}
	else if (type == e_center_relative)
	{
		Sprite* base = NULL;
		float maxArea = 0;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float area = m_sprites[i]->GetBounding()->Area();
			if (area > maxArea)
			{
				maxArea = area;
				base = m_sprites[i];
			}
		}

		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			if (sprite != base)
				sprite->SetTransform(base->GetPosition(), sprite->GetAngle());
		}
	}
}

void MultiSpritesPropertyImpl::OnPosChange(float dx, float dy)
{
	SetWndDirtySJ::Instance()->SetDirty();

	EditAddRecordSJ::Instance()->Add(new TranslateSpriteAOP(m_sprites, Vector(dx, dy)));
	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		Sprite* spr = m_sprites[i];
		Vector pos = spr->GetPosition();
		pos.x += dx;
		pos.y += dy;
		spr->SetTransform(pos, spr->GetAngle());
	}
}

void MultiSpritesPropertyImpl::OnAngleChange(float angle)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		Sprite* spr = m_sprites[i];
		spr->SetTransform(spr->GetPosition(), spr->GetAngle() + angle);
	}
}

void MultiSpritesPropertyImpl::OnScaleChange(float dx, float dy)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		Sprite* s = m_sprites[i];
		Vector scale = s->GetScale();
		scale.x *= dx;
		scale.y *= dy;
		s->SetScale(scale);
	}
}

Vector MultiSpritesPropertyImpl::GetOverallCenter() const
{
	if (m_sprites.empty()) {
		return Vector(0, 0);
	}

	SetWndDirtySJ::Instance()->SetDirty();

	Vector center(0, 0);
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		center += m_sprites[i]->GetPosition();
	}
	center /= m_sprites.size();
	return center;
}

}