#include "MultiSpritesPropertyImpl.h"
#include "Sprite.h"
#include "Symbol.h"
#include "color_config.h"
#include "Math2D.h"
#include "panel_msg.h"
#include "SetSpriteColorAOP.h"
#include "SetSpritePosAOP.h"
#include "TranslateSpriteAOP.h"

#include <sprite2/RenderColor.h>
#include <sprite2/BoundingBox.h>

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
		sprites[i]->AddReference();
	}
	m_sprites = sprites;
}

MultiSpritesPropertyImpl::~MultiSpritesPropertyImpl()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->RemoveReference();
	}
}

std::string MultiSpritesPropertyImpl::GetTag() const
{
	if (m_sprites.empty()) {
		return "";
	}

	std::string tag = m_sprites[0]->GetTag();
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (tag != m_sprites[i]->GetTag()) {
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

	bool clip = m_sprites[0]->IsClip();
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (clip != m_sprites[i]->IsClip()) {
			return BOOL_3TYPE_LABELS[e_bool_different];
		}
	}
	return clip ? BOOL_3TYPE_LABELS[e_bool_true] : BOOL_3TYPE_LABELS[e_bool_false];
}

sm::vec2 MultiSpritesPropertyImpl::GetPosition() const
{
	if (m_sprites.empty()) {
		return sm::vec2();
	}

	sm::vec2 p = m_sprites[0]->GetPosition();
	for (size_t i = 1, n = m_sprites.size(); i < n; ++i)
	{
		if (p != m_sprites[i]->GetPosition())
		{
			p.Set(0, 0);
			break;
		}
	}
	return p;
}

s2::Color MultiSpritesPropertyImpl::GetMultiColor() const
{
	if (m_sprites.empty()) {
		return WHITE;
	}

	s2::Color col = m_sprites[0]->Color().mul;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->Color().mul) {
			return WHITE;
		}
	}
	return col;
}

s2::Color MultiSpritesPropertyImpl::GetAddColor() const
{
	if (m_sprites.empty()) {
		return BLACK;
	}
	
	s2::Color col = m_sprites[0]->Color().add;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->Color().add) {
			return BLACK;
		}
	}
	return col;
}

s2::Color MultiSpritesPropertyImpl::GetTransColorR() const
{
	if (m_sprites.empty()) {
		return RED;
	}

	s2::Color col = m_sprites[0]->Color().rmap;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->Color().rmap) {
			return RED;
		}
	}
	return col;
}

s2::Color MultiSpritesPropertyImpl::GetTransColorG() const
{
	if (m_sprites.empty()) {
		return GREEN;
	}

	s2::Color col = m_sprites[0]->Color().gmap;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->Color().gmap) {
			return GREEN;
		}
	}
	return col;
}

s2::Color MultiSpritesPropertyImpl::GetTransColorB() const
{
	if (m_sprites.empty()) {
		return BLUE;
	}

	s2::Color col = m_sprites[0]->Color().bmap;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->Color().bmap) {
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

sm::vec2 MultiSpritesPropertyImpl::GetScale() const
{
	if (m_sprites.empty()) {
		return sm::vec2(1, 1);
	}

	sm::vec2 scale = m_sprites[0]->GetScale();
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (scale != m_sprites[i]->GetScale()) {
			return sm::vec2(1, 1);
		}
	}
	return scale;
}

sm::bvec2 MultiSpritesPropertyImpl::GetMirror() const
{
	sm::bvec2 mirror(false, false);

	if (m_sprites.empty()) {
		return mirror;
	}

	mirror.x = m_sprites[0]->GetMirror().x;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (mirror.x != m_sprites[i]->GetMirror().x) {
			mirror.x = false;
			break;
		}
	}

	mirror.y = m_sprites[0]->GetMirror().y;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (mirror.y != m_sprites[i]->GetMirror().y) {
			mirror.y = false;
			break;
		}
	}

	return mirror;
}

bool MultiSpritesPropertyImpl::GetVisible() const
{
	if (m_sprites.empty()) {
		return true;
	}

	bool ret = m_sprites[0]->IsVisible();
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (ret != m_sprites[i]->IsVisible()) {
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

	bool ret = m_sprites[0]->IsEditable();
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (ret != m_sprites[i]->IsEditable()) {
			return true;
		}
	}
	return ret;
}

void MultiSpritesPropertyImpl::SetTag(const std::string& tag)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->SetTag(tag);
	}
}

void MultiSpritesPropertyImpl::SetClip(int clip)
{
	SetWndDirtySJ::Instance()->SetDirty();

	Bool3Type type = (Bool3Type)clip;
	if (type == e_bool_true) {
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_sprites[i]->SetClip(true);
		}
	} else if (type == e_bool_false) {
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_sprites[i]->SetClip(false);
		}
	}
}

void MultiSpritesPropertyImpl::SetPos(float x, float y)
{
	sm::vec2 pos(x, y);
	EditAddRecordSJ::Instance()->Add(new SetSpritePosAOP(m_sprites, pos));
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetPosition(pos);
	}
}

void MultiSpritesPropertyImpl::SetColorMul(const s2::Color& col)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		float alpha = m_sprites[i]->Color().mul.a;
		m_sprites[i]->Color().mul = col;
		m_sprites[i]->Color().mul.a = alpha;
	}
}

void MultiSpritesPropertyImpl::SetColorAdd(const s2::Color& col)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		float alpha = m_sprites[i]->Color().add.a;
		m_sprites[i]->Color().add = col;
		m_sprites[i]->Color().add.a = alpha;
	}
}

void MultiSpritesPropertyImpl::SetColorAlpha(int alpha)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Color().mul.a = alpha;
	}
}

void MultiSpritesPropertyImpl::SetAngle(bool overall, float angle)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall) 
	{
		sm::vec2 center = GetOverallCenter();
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			sm::vec2 pos = Math2D::RotateVector(spr->GetPosition() - center, angle);
			spr->SetPosition(pos);
			spr->SetAngle(angle);
		}
	} 
	else 
	{
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			spr->SetAngle(angle);
		}
	}
}

void MultiSpritesPropertyImpl::SetScale(bool overall, float sx, float sy)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall)
	{
		sm::vec2 center = GetOverallCenter();
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			sm::vec2 pos = spr->GetPosition() - center;
			pos.x *= sx;
			pos.y *= sy;
			spr->SetPosition(pos + center);
		}
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->SetScale(sm::vec2(sx, sy));
	}
}

void MultiSpritesPropertyImpl::SetMirrorX(bool overall, bool mirror)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall)
	{
		sm::vec2 center = GetOverallCenter();
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			sm::vec2 pos = spr->GetPosition() - center;
			pos.x = -pos.x;
			spr->SetPosition(pos + center);
			spr->SetAngle(-spr->GetAngle());
		}
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetMirror(sm::bvec2(mirror, spr->GetMirror().y));
	}
}

void MultiSpritesPropertyImpl::SetMirrorY(bool overall, bool mirror)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall)
	{
		sm::vec2 center = GetOverallCenter();
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			Sprite* spr = m_sprites[i];
			sm::vec2 pos = spr->GetPosition() - center;
			pos.y = -pos.y;
			spr->SetPosition(pos + center);
			spr->SetAngle(-spr->GetAngle());
		}
	}

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetMirror(sm::bvec2(spr->GetMirror().x, mirror));
	}
}

void MultiSpritesPropertyImpl::SetVisible(bool visible)
{
	SetWndDirtySJ::Instance()->SetDirty();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->SetVisible(visible);
	}
}

void MultiSpritesPropertyImpl::SetEditable(bool editable)
{
	SetWndDirtySJ::Instance()->SetDirty();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->SetEditable(editable);
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
			Sprite* spr = m_sprites[i];
			float l = spr->GetBounding()->GetSize().xmin;
			if (l < left)
				left = l;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* spr = m_sprites[i];
			float x = spr->GetPosition().x + left - spr->GetBounding()->GetSize().xmin;
			m_sprites[i]->SetPosition(sm::vec2(x, spr->GetPosition().y));
		}
	}
	else if (type == e_align_right)
	{
		float right = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* spr = m_sprites[i];
			float r = spr->GetBounding()->GetSize().xmax;
			if (r > right)
				right = r;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* spr = m_sprites[i];
			float x = spr->GetPosition().x + right - spr->GetBounding()->GetSize().xmax;
			m_sprites[i]->SetPosition(sm::vec2(x, spr->GetPosition().y));
		}
	}
	else if (type == e_align_up)
	{
		float up = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* spr = m_sprites[i];
			float u = spr->GetBounding()->GetSize().ymax;
			if (u > up)
				up = u;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* spr = m_sprites[i];
			float y = spr->GetPosition().y + up - spr->GetBounding()->GetSize().ymax;
			m_sprites[i]->SetPosition(sm::vec2(spr->GetPosition().x, y));
		}
	}
	else if (type == e_align_down)
	{
		float down = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* spr = m_sprites[i];
			float d = spr->GetBounding()->GetSize().ymin;
			if (d < down)
				down = d;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* spr = m_sprites[i];
			float y = spr->GetPosition().y + down - spr->GetBounding()->GetSize().ymin;
			m_sprites[i]->SetPosition(sm::vec2(spr->GetPosition().x, y));
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
			m_sprites[i]->SetPosition(sm::vec2(left, sprite->GetPosition().y));
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
			m_sprites[i]->SetPosition(sm::vec2(right, sprite->GetPosition().y));
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
			m_sprites[i]->SetPosition(sm::vec2(sprite->GetPosition().x, up));
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
			m_sprites[i]->SetPosition(sm::vec2(sprite->GetPosition().x, down));
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
			sprite->SetPosition(sm::vec2(left + space * i, sprite->GetPosition().y));
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
			sprite->SetPosition(sm::vec2(sprite->GetPosition().x, down + space * i));
		}
	}
	else if (type == e_center_relative)
	{
		Sprite* base = NULL;
		float maxArea = 0;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			sm::vec2 sz = m_sprites[i]->GetBounding()->GetSize().Size();
			float area = sz.x * sz.y;
			if (area > maxArea)
			{
				maxArea = area;
				base = m_sprites[i];
			}
		}

		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			if (sprite != base) {
				sprite->SetPosition(base->GetPosition());
			}
		}
	}
}

void MultiSpritesPropertyImpl::OnPosChange(float dx, float dy)
{
	SetWndDirtySJ::Instance()->SetDirty();

	EditAddRecordSJ::Instance()->Add(new TranslateSpriteAOP(m_sprites, sm::vec2(dx, dy)));
	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		Sprite* spr = m_sprites[i];
		spr->Translate(sm::vec2(dx, dy));
	}
}

void MultiSpritesPropertyImpl::OnAngleChange(float angle)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		Sprite* spr = m_sprites[i];
		spr->Rotate(angle);
	}
}

void MultiSpritesPropertyImpl::OnScaleChange(float dx, float dy)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		Sprite* s = m_sprites[i];
		sm::vec2 scale = s->GetScale();
		scale.x *= dx;
		scale.y *= dy;
		s->SetScale(scale);
	}
}

sm::vec2 MultiSpritesPropertyImpl::GetOverallCenter() const
{
	if (m_sprites.empty()) {
		return sm::vec2(0, 0);
	}

	SetWndDirtySJ::Instance()->SetDirty();

	sm::vec2 center(0, 0);
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		center += m_sprites[i]->GetPosition();
	}
	center /= m_sprites.size();
	return center;
}

}