#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CColliderTriangle.h"
#include "CColliderCapsule.h"
#include "CColliderMesh.h"
#include "CObjectBase.h"
#include "Maths.h"

// �R���X�g���N�^
CCollider::CCollider(CObjectBase* owner, ELayer layer, EColliderType type,
	bool isKinematic, float weight)
	: mLayer(layer)
	, mType(type)
	, mpOwner(owner)
	, mIsEnable(true)
	, mIsKinematic(isKinematic)
	, mWeight(weight)
	, mCollisionLayers(~0)
	, mCollisionTags(~0)
	, mpAttachMtx(nullptr)
{
	// �R���W�������X�g�ɒǉ�
	CCollisionManager::Instance()->Add(this);
}

// �f�X�g���N�^
CCollider::~CCollider()
{
	// �R���W�������X�g����폜
	CCollisionManager::Instance()->Remove(this);
}

// �Փ˔��背�C���[���擾
ELayer CCollider::Layer() const
{
	return mLayer;
}

// �R���C�_�[�̎�ނ��擾
EColliderType CCollider::Type() const
{
	return mType;
}

// �R���C�_�[�̎������Ԃ�
CObjectBase* CCollider::Owner() const
{
	return mpOwner;
}

// �R���C�_�[�̎�����̃^�O���擾
ETag CCollider::Tag() const
{
	// �����傪���݂���΁A������̃^�O��Ԃ�
	if (mpOwner == nullptr) return ETag::eNone;
	return mpOwner->Tag();
}

// �R���C�_�[���L�����ǂ�����ݒ�
void CCollider::SetEnable(bool isEnable)
{
	mIsEnable = isEnable;
}

// �R���C�_�[���L�����ǂ������擾
bool CCollider::IsEnable() const
{
	return mIsEnable;
}

// �Փˎ��̉����߂��̉e�����󂯂邩�ǂ�����ݒ�
void CCollider::SetKinematic(bool iskinematic)
{
	mIsKinematic = iskinematic;
}

// �Փˎ��̉����߂��̉e�����󂯂邩�ǂ���
bool CCollider::IsKinematic() const
{
	return mIsKinematic;
}

// �R���C�_�[�̏d�ʂ�ݒ�
void CCollider::SetWeight(float weight)
{
	mWeight = weight;
}

// �R���C�_�[�̏d�ʂ��擾
float CCollider::GetWeight() const
{
	return mWeight;
}

// �w�肵���R���C�_�[�ƏՓ˔�����s�����ǂ������擾
bool CCollider::IsCollision(CCollider* col) const
{
	// �������g�������͑���̃R���C�_�[���L���łȂ���΁A
	// �Փ˔�����s��Ȃ�
	if (col == nullptr) return false;
	if (!col->IsEnable()) return false;
	if (!mIsEnable) return false;

	// �����R���C�_�[�������́A�����傪�����ł���΁A
	// �Փ˔�����s��Ȃ�
	if (col == this) return false;
	if (col->Owner() != nullptr || mpOwner != nullptr)
	{
		if (col->Owner() == mpOwner) return false;
	}

	// �Փ˔�����s�����C���[��
	if (!IsCollisionLayer(col->Layer())) return false;
	// �Փ˔�����s���I�u�W�F�N�g�^�O��
	if (!IsCollisionTag(col->Tag())) return false;

	// �S�Ă̏����𖞂������ꍇ�͏Փ˔�����s��
	return true;
}

// �Փ˔�����s�����C���[��ݒ�
void CCollider::SetCollisionLayers(Layers layers)
{
	mCollisionLayers = 0;
	for (auto&& layer : layers)
	{
		mCollisionLayers |= 1 << (int)layer;
	}
}

// �w�肵�����C���[�Ƃ̏Փ˔�����s�����ǂ�����ݒ�
void CCollider::SetCollisionLayer(ELayer layer, bool isCollision)
{
	if (isCollision) mCollisionLayers |= 1 << (int)layer;
	else mCollisionLayers &= ~(1 << (int)layer);
}

// �w�肵�����C���[�ƏՓ˔�����s�����ǂ������擾
bool CCollider::IsCollisionLayer(ELayer layer) const
{
	return (mCollisionLayers & 1 << (int)layer) != 0;
}

// �Փ˔�����s���I�u�W�F�N�g�^�O��ݒ�
void CCollider::SetCollisionTags(Tags tags)
{
	mCollisionTags = 0;
	for (auto&& tag : tags)
	{
		mCollisionTags |= 1 << (int)tag;
	}
}

// �w�肵���I�u�W�F�N�g�^�O�Ƃ̏Փ˔�����s�����ǂ�����ݒ�
void CCollider::SetCollisionTag(ETag tag, bool isCollision)
{
	if (isCollision) mCollisionTags |= 1 << (int)tag;
	else mCollisionTags &= ~(1 << (int)tag);
}

// �w�肵���I�u�W�F�N�g�^�O�ƏՓ˔�����s�����ǂ������擾
bool CCollider::IsCollisionTag(ETag tag) const
{
	return (mCollisionTags & 1 << (int)tag) != 0;
}

// �w�肵���s��ɃR���C�_�[�𕍑�������
void CCollider::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}

// �s����擾
CMatrix CCollider::Matrix() const
{
	CMatrix m = CTransform::Matrix();
	// ����������s�񂪐ݒ肳��Ă����
	if (mpAttachMtx != nullptr)
	{
		// ���̍s��ɕ�������
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		m = sm * mAttachMtx * m;
	}
	// �����傪�ݒ肳��Ă����
	else if (mpOwner != nullptr)
	{
		// ������̍s��ɕ���
		m = mpOwner->Matrix() * m;
	}
	return m;
}

// �o�E���f�B���O�{�b�N�X���擾
CBounds CCollider::Bounds() const
{
	return mBounds;
}

// �R���C�_�[�X�V
void CCollider::Update()
{
	// ��������s�񂪐ݒ肳��Ă���΁A
	if (mpAttachMtx != nullptr)
	{
		// ��������s��������o�ϐ��ɃR�s�[
		mAttachMtx = *mpAttachMtx;
	}
}

// ��`���m�̏Փ˔���
bool CCollider::CollisionRect(const CRect& rect1, const CRect& rect2)
{
	if (rect1.Left() <= rect2.Right() && rect1.Right() >= rect2.Left() &&
		rect1.Top() <= rect2.Bottom() && rect1.Bottom() >= rect2.Top())
	{
		return true;
	}
	return false;
}

// ��`�Ɠ_�̏Փ˔���
bool CCollider::CollisionRectPoint(const CRect& rect, const CVector2 point)
{
	if (rect.Left() <= point.X() && rect.Right() >= point.X() &&
		rect.Top() <= point.Y() && rect.Bottom() >= point.Y())
	{
		return true;
	}
	return false;
}

// �R���C�_�[�̐ݒ�
void CCollider::Set(CObjectBase* owner, ELayer layer)
{
	mpOwner = owner;
	mLayer = layer;
}

// �O�p�`�ƎO�p�`�̏Փ˔���
bool CCollider::CollisionTriangle(const CVector& t00, const CVector& t01, const CVector& t02,
	const CVector& t10, const CVector& t11, const CVector& t12,
	CHitInfo* hit)
{
	return false;
}

// �O�p�`�Ɛ����̏Փ˔���
bool CCollider::CollisionTriangleLine(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& ls, const CVector& le,
	CHitInfo* h, bool isLeftMain)
{
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector normal = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	//�O�p�̒��_��������n�_�ւ̃x�N�g�������߂�
	CVector v0sv = ls - t0;
	//�O�p�̒��_��������I�_�ւ̃x�N�g�������߂�
	CVector v0ev = le - t0;
	//�������ʂƌ������Ă��邩���ςŊm�F����
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//�v���X�͌������ĂȂ�
	if (dots * dote >= 0.0f) {
		//�Փ˂��ĂȂ��i�����s�v�j
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�����͖ʂƌ������Ă���
	//�ʂƐ����̌�_�����߂�
	//��_�̌v�Z
	CVector cross = ls + (le - ls) * (abs(dots) / (abs(dots) + abs(dote)));

	//��_���O�p�`���Ȃ�Փ˂��Ă���
	if (!IsInsideTriangle(cross, t0, t1, t2, normal))
	{
		//�O�p�`�O�Ȃ̂ŁA�Փ˂��ĂȂ�
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�q�b�g���Ɍ�_��ݒ�
	h->cross = cross;

	//�����l�v�Z�i�Փ˂��Ȃ��ʒu�܂Ŗ߂��j
	if (dots < 0.0f) {
		//�n�_������
		h->adjust = normal * dots;
	}
	else {
		//�I�_������
		h->adjust = normal * dote;
	}
	if (!isLeftMain) h->adjust = -h->adjust;
	return true;
}

// �O�p�`�ƃJ�v�Z���̏Փ˔���
bool CCollider::CollisionTriangleCapsule(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& cs, const CVector& ce, float cr,
	CHitInfo* h, bool isLeftMain)
{
	// �O�p�`�̖@�����Z�o
	CVector n = CVector::Cross(t1 - t0, t2 - t0).Normalized();

	// �J�v�Z���̎n�_����O�p�`�̒��_�܂ł̃x�N�g�������߂�
	CVector vs = cs - t0;
	// �J�v�Z���̏I�_����O�p�`�̒��_�܂ł̃x�N�g�������߂�
	CVector ve = ce - t0;

	// �e�x�N�g���ƎO�p�`�̖@���Ƃ̓��ς����߂�
	float d1 = CVector::Dot(vs, n);
	float d2 = CVector::Dot(ve, n);

	// �p�^�[���@
	// �J�v�Z�����\������O�p�`���������Ă�����A�����ƎO�p�`�̏Փ˔�����s��
	// �i�e���ς̌��ʂ���Z���ă}�C�i�X�̏ꍇ�́A
	// �@�J�v�Z�����\������������O�p�`�ƌ������Ă���j
	if (d1 * d2 < 0.0f)
	{
		// �����ƎO�p�`�̏Փ˔�����s��
		if (CollisionTriangleLine(t0, t1, t2, cs, ce, h, isLeftMain))
		{
			// �Փ˂��Ă����ꍇ�́A�����߂��x�N�g����
			// �J�v�Z���̔��a���̒�����ǉ����ĕԂ�
			h->adjust += h->adjust.Normalized() * cr;
			return true;
		}
	}


	// �n�_�ƏI�_�𐂒��ɉ��낵���ŋߓ_�����߂�
	CVector c1 = cs - n * d1;
	CVector c2 = ce - n * d2;
	// �n�_�ƏI�_�̉����߂��ʂ����߂�
	CVector v1, v2;
	if (fabsf(d1) <= cr) v1 = (c1 + n * cr) - cs;
	if (fabsf(d2) <= cr) v2 = (c2 + n * cr) - ce;

	// �p�^�[���A
	// �J�v�Z�����\����������̎n�_�ƏI�_����O�p�`�ւ̋��������߁A
	// �J�v�Z���̔��a���߂��i�߂荞��ł���j�ꍇ�́A�����߂�
	{
		float l1 = FLT_MAX, l2 = FLT_MAX;
		// ���߂��ŋߓ_���O�p�`�̓������A���������a�ȉ��ł���΁A
		// �����Ɖ����߂��x�N�g�������߂�
		if (IsInsideTriangle(c1, t0, t1, t2, n) && fabsf(d1) <= cr) l1 = v1.LengthSqr();
		if (IsInsideTriangle(c2, t0, t1, t2, n) && fabsf(d2) <= cr) l2 = v2.LengthSqr();

		if (l1 < FLT_MAX || l2 < FLT_MAX)
		{
			if (l1 < FLT_MAX && l2 < FLT_MAX) h->adjust = l1 >= l2 ? v1 : v2;
			else h->adjust = l1 < FLT_MAX ? v1 : v2;
			if (isLeftMain) h->adjust = -h->adjust;
			return true;
		}
	}

	// �p�^�[���B
	// �J�v�Z�����\����������ƎO�p�`�̊e�ӂ̋��������߁A
	// �J�v�Z���̔��a���߂��ꍇ�͉����߂�
	{
		float e1 = CalcDistanceLine(t0, t1, cs, ce);
		float e2 = CalcDistanceLine(t1, t2, cs, ce);
		float e3 = CalcDistanceLine(t2, t0, cs, ce);
		if (std::min(std::min(e1, e2), e3) <= cr)
		{
			if (e1 <= e2 && e1 <= e3)
			{
				CVector cv = CVector::Cross(t1 - t0, ce - cs).Normalized();
				if (cv.LengthSqr() == 0.0f)
				{
					CVector nv = (t1 - t0).Normalized();
					cv = (t0 + nv * CVector::Dot(cs - t0, nv) - cs).Normalized();
				}
				h->adjust = cv * (cr - e1);
			}
			else if (e2 <= e1 && e2 <= e3)
			{
				CVector cv = CVector::Cross(t2 - t1, ce - cs).Normalized();
				if (cv.LengthSqr() == 0.0f)
				{
					CVector nv = (t2 - t1).Normalized();
					cv = (t1 + nv * CVector::Dot(cs - t1, nv) - cs).Normalized();
				}
				h->adjust = cv * (cr - e2);
			}
			else
			{
				CVector cv = CVector::Cross(t0 - t2, ce - cs).Normalized();
				if (cv.LengthSqr() == 0.0f)
				{
					CVector nv = (t0 - t2).Normalized();
					cv = (t2 + nv * CVector::Dot(cs - t2, nv) - cs).Normalized();
				}
				h->adjust = cv * (cr - e3);
			}
			if (!isLeftMain) h->adjust = -h->adjust;
			return true;
		}
	}

	h->adjust = CVector::zero;
	return false;
}

// �O�p�`�Ɠ_�̏Փ˔���
bool CCollider::CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& tn, const CVector& p)
{
	if (CVector::Dot(CVector::Cross(t1 - t0, p - t0), tn) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t2 - t1, p - t1), tn) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t0 - t2, p - t2), tn) < 0.0f) return false;
	return true;
}

// �O�p�`�Ɠ_�̏Փ˔���
bool CCollider::CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& p)
{
	CVector n = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	return CollisionTrianglePoint(t0, t1, t2, n, p);
}

// �O�p�`�Ƌ��̏Փ˔���
bool CCollider::CollisionTriangleSphere(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& sp, const float sr,
	CHitInfo* h, bool isLeftMain)
{
	// �O�p�`�̖@�������߂�
	CVector n = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	// �@���̒�����0�ł���΁A�����ȎO�p�`�Ȃ��߁A�Փ˂��Ă��Ȃ�
	if (n.LengthSqr() <= 0.0f) return false;

	// �O�p�`���狅�܂ł̋��������߂āA
	// ���������̔��a��藣��Ă�����A�Փ˂��Ă��Ȃ�
	CVector v = sp - t0;
	float dist = CVector::Dot(v, n);
	if (fabsf(dist) > sr) return false;

	// ���̒��S�_����O�p�`�֐����ɉ��낵���_�����߂�
	CVector point = sp + (-n * dist);
	// ���߂��_���O�p�`�͈͓̔������ׂ�
	if (CollisionTrianglePoint(t0, t1, t2, n, point))
	{
		// �͈͓��ł���΁A�Փ˂��Ă���̂ŁA
		// �����߂��ʂ��v�Z
		float l = (sr - fabsf(dist)) * (dist < 0.0f ? -1.0f : 1.0f);
		h->adjust = n * l * (isLeftMain ? -1.0f : 1.0f);
		return true;
	}

	// �����ɉ��낵���_���O�p�`�͈̔͊O�̏ꍇ�A
	// �e�ӂƋ����Փ˂��Ă��Ȃ����m�F

	// �O�p�`�̊e�ӂƂ̍ŒZ���������߁A
	// ���a��菬�����ꍇ�͏Փ˂��Ă��邽�߁A
	// ���̕ӂƂ̉����߂��ʂ��v�Z���ĕԂ�

	// �O�p�`�̒��_0���璸�_1�܂ł̕�
	dist = CalcDistancePointToLine(sp, t0, t1);
	if (dist <= sr)
	{
		float l = sr - dist;
		h->adjust = n * l * (isLeftMain ? -1.0f : 1.0f);
		return true;
	}
	// �O�p�`�̒��_1���璸�_2�܂ł̕�
	dist = CalcDistancePointToLine(sp, t1, t2);
	if (dist <= sr)
	{
		float l = sr - dist;
		h->adjust = n * l * (isLeftMain ? -1.0f : 1.0f);
		return true;
	}
	// �O�p�`�̒��_2���璸�_0�܂ł̕�
	dist = CalcDistancePointToLine(sp, t2, t0);
	if (dist <= sr)
	{
		float l = sr - dist;
		h->adjust = n * l * (isLeftMain ? -1.0f : 1.0f);
		return true;
	}

	// �O�p�`�͈̔͊O���A�e�ӂƂ��Փ˂��Ă��Ȃ��ꍇ�́A
	// ���S�ɏՓ˂��Ă��Ȃ�
	return false;
}

// ���Ƌ��̏Փ˔���
bool CCollider::CollisionSphere(const CVector& sp0, const float sr0,
	const CVector& sp1, const float sr1, CHitInfo* hit)
{
	//���S���璆�S�ւ̃x�N�g�������߂�
	CVector vec = sp0 - sp1;
	float length = vec.Length();
	//���S�̋��������a�̍��v��菬�����ƏՓ�
	float sum = sr0 + sr1;
	if (sum > length) {
		hit->adjust = vec.Normalized() * (sum - length);
		//�Փ˂��Ă���
		return  true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	//�Փ˂��Ă��Ȃ�
	return false;
}

// ���Ɛ����̏Փ˔���
bool CCollider::CollisionSphereLine(const CVector& sp, const float sr,
	const CVector& ls, const CVector& le,
	CHitInfo* hit, bool isLeftMain)
{
	CVector nearest;
	float length = CalcDistancePointToLine(sp, ls, le, &nearest);
	if (length < sr)
	{
		CVector n = (sp - nearest).Normalized() * (isLeftMain ? 1.0f : -1.0f);
		hit->adjust = n * (sr - length);

		CVector v = (le - ls).Normalized();
		CVector p = (sp - ls).Normalized();
		float a = CVector::Dot(v, v);
		float b = CVector::Dot(v, p);
		float c = CVector::Dot(p, p) - sr * sr;
		if (a != 0.0f) hit->cross = ls + v * ((b - sqrtf(b * b - a * c)) / a);
		else hit->cross = CVector::zero;
		return true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	return false;
}

// ���ƃJ�v�Z���̏Փ˔���
bool CCollider::CollisionSphereCapsule(
	const CVector& sp, const float sr,
	const CVector& cs, const CVector& ce, float cr,
	CHitInfo* hit, bool isLeftMain)
{
	CVector nearest;
	float length = CalcDistancePointToLine(sp, cs, ce, &nearest);
	if (length < sr + cr)
	{
		CVector n = (sp - nearest).Normalized() * (isLeftMain ? 1.0f : -1.0f);
		hit->adjust = n * ((sr + cr) - length);

		CVector v = (ce - cs).Normalized();
		CVector p = (sp - cs).Normalized();
		float a = CVector::Dot(v, v);
		float b = CVector::Dot(v, p);
		float c = CVector::Dot(p, p) - sr * sr;
		if (a != 0.0f) hit->cross = cs + v * ((b - sqrtf(b * b - a * c)) / a);
		else hit->cross = CVector::zero;
		return true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	return false;
}

// �����Ɛ����̏Փ˔���
bool CCollider::CollisionLine(const CVector& ls0, const CVector& le0,
	const CVector& ls1, const CVector& le1, CHitInfo* hit)
{
	//TODO:�����l�̑Ή�
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector V0 = le0 - ls0;
	CVector V1 = le1 - ls1;

	CVector S1E1 = le0 - ls0;
	CVector S2E2 = le1 - ls1;
	CVector CD = CVector::Cross(V0, V1).Normalized();

	CVector S1S2 = ls1 - ls0;
	CVector S1E2 = le1 - ls0;
	CVector S2S1 = ls0 - ls1;
	CVector S2E1 = le0 - ls1;

	float length = 0.0f;
	float d1 = S1E1.Cross(S1S2).Dot(S1E1.Cross(S1E2));
	float d2 = S2E2.Cross(S2S1).Dot(S2E2.Cross(S2E1));
	if (d1 < 0 && d2 < 0)
	{
		length = abs(S1S2.Dot(CD));
	}
	else
	{
		float length1 = CalcDistancePointToLine(ls0, ls1, le1);
		float length2 = CalcDistancePointToLine(le0, ls1, le1);
		float length3 = CalcDistancePointToLine(ls1, ls0, le0);
		float length4 = CalcDistancePointToLine(le1, ls0, le0);
		length = fminf(fminf(length1, length2), fminf(length3, length4));
	}

	if (length < 0.1f + 0.1f)
	{
		return true;
	}

	return false;
}

// �J�v�Z���Ɛ����̏Փ˔���
bool CCollider::CollisionCapsuleLine(
	const CVector& cs, const CVector& ce, float cr,
	const CVector& ls, const CVector& le,
	CHitInfo* hit, bool isLeftMain)
{
	//TODO:�����l�̑Ή�
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector V0 = le - ls;
	CVector V1 = ce - cs;

	CVector S1E1 = le - ls;
	CVector S2E2 = ce - cs;
	CVector CD = CVector::Cross(V0, V1).Normalized();

	CVector S1S2 = cs - ls;
	CVector S1E2 = ce - ls;
	CVector S2S1 = ls - cs;
	CVector S2E1 = le - cs;

	float length = 0.0f;
	float d1 = S1E1.Cross(S1S2).Dot(S1E1.Cross(S1E2));
	float d2 = S2E2.Cross(S2S1).Dot(S2E2.Cross(S2E1));
	if (d1 < 0 && d2 < 0)
	{
		length = abs(S1S2.Dot(CD));
	}
	else
	{
		float length1 = CalcDistancePointToLine(ls, cs, ce);
		float length2 = CalcDistancePointToLine(le, cs, ce);
		float length3 = CalcDistancePointToLine(cs, ls, le);
		float length4 = CalcDistancePointToLine(ce, ls, le);
		length = fminf(fminf(length1, length2), fminf(length3, length4));
	}

	if (length < cr)
	{
		CVector n = CVector::zero;
		if (CD.LengthSqr() == 0.0f)
		{
			n = CVector::Cross(V0, CVector::forward);
			if (n.LengthSqr() == 0.0f)
			{
				n = CVector::Cross(V0, CVector::up);
			}
		}
		else
		{
			n = CD;
		}

		n = (n.Normalized() * S1S2.Dot(n)).Normalized();
		hit->adjust = n * (cr - length);
		return true;
	}

	return false;
}

// �J�v�Z���ƃJ�v�Z���̏Փ˔���
bool CCollider::CollisionCapsule(const CVector& cs0, const CVector& ce0, float cr0, const CVector& cs1, const CVector& ce1, float cr1, CHitInfo* hit)
{
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector V0 = ce0 - cs0;
	CVector V1 = ce1 - cs1;
	CVector VN0 = V0.Normalized();
	CVector VN1 = V1.Normalized();

	CVector S1E1 = ce0 - cs0;
	CVector S2E2 = ce1 - cs1;
	CVector CD = CVector::Cross(V0, V1).Normalized();

	CVector S1S2 = cs1 - cs0;
	CVector S1E2 = ce1 - cs0;
	CVector S2S1 = cs0 - cs1;
	CVector S2E1 = ce0 - cs1;

	CVector cross = CVector::zero;

	float length = 0.0f;
	float d1 = S1E1.Cross(S1S2).Dot(S1E1.Cross(S1E2));
	float d2 = S2E2.Cross(S2S1).Dot(S2E2.Cross(S2E1));
	if (d1 < 0 && d2 < 0)
	{
		length = abs(S1S2.Dot(CD));
		float d = CVector::Dot(VN0, VN1);
		float dn0 = CVector::Dot(S1S2, VN0);
		float dn1 = CVector::Dot(S1S2, VN1);
		float r = 1.0f - d * d;
		if (r != 0.0f)
		{
			float t1 = (dn0 - d * dn1) / r;
			float t2 = (d * dn0 - dn1) / r;
			CVector p1 = cs0 + VN0 * t1;
			CVector p2 = cs1 + VN1 * t2;
			cross = (p1 + p2) * 0.5f;
		}
	}
	else
	{
		CVector n1, n2, n3, n4;
		float length1 = CalcDistancePointToLine(cs0, cs1, ce1, &n1);
		float length2 = CalcDistancePointToLine(ce0, cs1, ce1, &n2);
		float length3 = CalcDistancePointToLine(cs1, cs0, ce0, &n3);
		float length4 = CalcDistancePointToLine(ce1, cs0, ce0, &n4);
		length = fminf(fminf(length1, length2), fminf(length3, length4));

		if (length == length1) cross = (cs0 + n1) * 0.5f;
		else if (length == length2) cross = (ce0 + n2) * 0.5f;
		else if (length == length3) cross = (cs1 + n3) * 0.5f;
		else cross = (ce1 + n4) * 0.5f;
	}

	if (length < cr0 + cr1)
	{
		CVector n = CVector::zero;
		if (CD.LengthSqr() == 0.0f)
		{
			n = CVector::Cross(V0, CVector::forward);
			if (n.LengthSqr() == 0.0f)
			{
				n = CVector::Cross(V0, CVector::up);
			}
		}
		else
		{
			n = CD;
		}

		n = -(n.Normalized() * S1S2.Dot(n)).Normalized();
		hit->adjust = n * ((cr0 + cr1) - length);
		hit->cross = cross;
		return true;
	}

	return false;
}

// ���b�V���Ɛ����̏Փ˔���
bool CCollider::CollisionMeshLine(const std::list<STVertexData>& tris,
	const CVector& ls, const CVector& le, const CBounds& lb,
	CHitInfo* hit, bool isLeftMain)
{
	bool ret = false;
	CVector adjust = CVector::zero;
	CVector cross = CVector::zero;
	float nearDist = 0.0f;
	bool isFirst = true;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, lb)) continue;
		if (CollisionTriangleLine(v.wv.V[0], v.wv.V[1], v.wv.V[2], ls, le, hit, isLeftMain))
		{
			hit->tris.push_back(v.wv);

			CVector adj = hit->adjust;
			adjust.X(abs(adjust.X()) > abs(adj.X()) ? adjust.X() : adj.X());
			adjust.Y(abs(adjust.Y()) > abs(adj.Y()) ? adjust.Y() : adj.Y());
			adjust.Z(abs(adjust.Z()) > abs(adj.Z()) ? adjust.Z() : adj.Z());

			if (isFirst)
			{
				cross = hit->cross;
				nearDist = (cross - ls).Length();
				isFirst = false;
			}
			else
			{
				float dist = (hit->cross - ls).Length();
				if (dist < nearDist)
				{
					cross = hit->cross;
					nearDist = dist;
				}
			}

			ret = true;
		}
	}
	hit->adjust = adjust;
	hit->cross = cross;
	hit->dist = nearDist;
	return ret;
}

// ���b�V���Ƌ��̏Փ˔���
bool CCollider::CollisionMeshSpehre(const std::list<STVertexData>& tris,
	CColliderSphere* sphereCol, CHitInfo* hit, bool isLeftMain)
{
	CVector sp;
	float sr;
	sphereCol->Get(&sp, &sr);

	bool ret = false;
	CVector adjust = CVector::zero;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, sphereCol->Bounds())) continue;
		if (CollisionTriangleSphere(v.wv.V[0], v.wv.V[1], v.wv.V[2], sp, sr, hit, isLeftMain))
		{
			adjust.Y(fabsf(adjust.Y()) > fabsf(hit->adjust.Y()) ? adjust.Y() : hit->adjust.Y());
			adjust.X(fabsf(adjust.X()) > fabsf(hit->adjust.X()) ? adjust.X() : hit->adjust.X());
			adjust.Z(fabsf(adjust.Z()) > fabsf(hit->adjust.Z()) ? adjust.Z() : hit->adjust.Z());
			hit->tris.push_back(v.wv);
			ret = true;
		}
	}
	if (ret)
	{
		hit->adjust = adjust;
	}
	return ret;
}

// ���b�V���ƎO�p�`�̏Փ˔���
bool CCollider::CollisionMeshTriangle(const std::list<STVertexData>& tris,
	CColliderTriangle* triCol, CHitInfo* hit, bool isLeftMain)
{
	CVector t0, t1, t2;
	triCol->Get(&t0, &t1, &t2);

	bool ret = false;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, triCol->Bounds())) continue;
		CVector v00 = isLeftMain ? v.wv.V[0] : t0;
		CVector v01 = isLeftMain ? v.wv.V[1] : t1;
		CVector v02 = isLeftMain ? v.wv.V[2] : t2;
		CVector v10 = isLeftMain ? t0 : v.wv.V[0];
		CVector v11 = isLeftMain ? t1 : v.wv.V[1];
		CVector v12 = isLeftMain ? t2 : v.wv.V[2];
		if (CollisionTriangle(v00, v01, v02, v10, v11, v12, hit))
		{
			hit->tris.push_back(v.wv);
			ret = true;
		}
	}
	return ret;
}

// ���b�V���ƃJ�v�Z���̏Փ˔���
bool CCollider::CollisionMeshCapsule(const std::list<STVertexData>& tris,
	CColliderCapsule* capsuleCol, CHitInfo* hit, bool isLeftMain)
{
	CVector cs, ce;
	capsuleCol->Get(&cs, &ce);
	float cr = capsuleCol->Radius();

	bool ret = false;
	CVector adjust = CVector::zero;
	CVector cross = CVector::zero;
	float nearDist = 0.0f;
	bool isFirst = true;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, capsuleCol->Bounds())) continue;
		if (CollisionTriangleCapsule(v.wv.V[0], v.wv.V[1], v.wv.V[2], cs, ce, cr, hit, isLeftMain))
		{
			hit->tris.push_back(v.wv);

			CVector adj = hit->adjust;
			adjust.X(abs(adjust.X()) > abs(adj.X()) ? adjust.X() : adj.X());
			adjust.Y(abs(adjust.Y()) > abs(adj.Y()) ? adjust.Y() : adj.Y());
			adjust.Z(abs(adjust.Z()) > abs(adj.Z()) ? adjust.Z() : adj.Z());

			if (isFirst)
			{
				cross = hit->cross;
				nearDist = (cross - cs).Length();
				isFirst = false;
			}
			else
			{
				float dist = (hit->cross - cs).Length();
				if (dist < nearDist)
				{
					cross = hit->cross;
					nearDist = dist;
				}
			}

			ret = true;
		}
	}
	hit->adjust = adjust;
	hit->cross = cross;
	hit->dist = nearDist;
	return ret;
}

float CCollider::CalcDistancePointToLine(const CVector& point, const CVector& lineS, const CVector& lineE, CVector* nearest)
{
	CVector SE = lineE - lineS;
	float t = (point - lineS).Dot(SE.Normalized());

	CVector C;
	if (t < 0) C = lineS;
	else if (t > SE.Length()) C = lineE;
	else
	{
		C = lineS + SE.Normalized() * t;
	}

	if (nearest != nullptr) *nearest = C;

	return (C - point).Length();
}

float CCollider::CalcDistanceLine(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2)
{
	// �������m���������邩�ǂ����ōŒZ�����̔����ς���
	CVector SE1 = e1 - s1;
	CVector SE2 = e2 - s2;
	float d1 = CVector::Dot(CVector::Cross(SE1, s2 - s1), CVector::Cross(SE1, e2 - s1));
	float d2 = CVector::Dot(CVector::Cross(SE2, s1 - s2), CVector::Cross(SE2, e1 - s2));
	// �������m���������Ă���ꍇ
	if (d1 < 0.0f && d2 < 0.0f)
	{
		CVector cv = CVector::Cross(SE1, SE2).Normalized();
		return fabsf(CVector::Dot(cv, s2 - s1));
	}
	// �������m���������Ă��Ȃ��ꍇ
	else
	{
		float l1 = CalcDistancePointToLine(s1, s2, e2);
		float l2 = CalcDistancePointToLine(e1, s2, e2);
		float l3 = CalcDistancePointToLine(s2, s1, e1);
		float l4 = CalcDistancePointToLine(e2, s1, e1);
		return std::min(std::min(l1, l2), std::min(l3, l4));
	}
}

bool CCollider::IsInsideTriangle(const CVector& p, const CVector& t0, const CVector& t1, const CVector& t2, const CVector& n)
{
	// �O�p�`�̊e�ӂƎO�p�`�̊e���_����_�܂ł̃x�N�g����
	// �����ȃx�N�g�����O�ρiCVector::Cross�j�ŋ��߂�B

	// �O�p�`�̖@���Ƌ��߂��x�N�g���̓��ρiCVector::Dot�j��
	// ���x�N�g���̌����𒲂ׂē��������������Ă��邩�𒲂ׂ�B

	// ������O�p�`�̎O�ӕ��s���A�S�ď����𖞂������ꍇ�́A
	// �O�p�`�̓����Ɣ��f����B
	if (CVector::Dot(CVector::Cross(t1 - t0, p - t0), n) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t2 - t1, p - t1), n) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t0 - t2, p - t2), n) < 0.0f) return false;
	return true;
}

bool CCollider::Collision(CCollider* c0, CCollider* c1, CHitInfo* hit)
{
	switch (c0->Type())
	{
	case EColliderType::eLine:
	{
		CColliderLine* line0 = dynamic_cast<CColliderLine*>(c0);
		CVector ls0, le0;
		line0->Get(&ls0, &le0);

		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line1 = dynamic_cast<CColliderLine*>(c1);
			CVector ls1, le1;
			line1->Get(&ls1, &le1);
			return CollisionLine(ls0, le0, ls1, le1, hit);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionSphereLine(sp, sr, ls0, le0, hit, false);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleLine(t0, t1, t2, ls0, le0, hit, false);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs, ce;
			capsule->Get(&cs, &ce);
			float cr = capsule->Radius();
			return CollisionCapsuleLine(cs, ce, cr, ls0, le0, hit, false);
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshLine(tris, ls0, le0, line0->Bounds(), hit, false);
		}
		}
		break;
	}
	case EColliderType::eSphere:
	{
		CColliderSphere* sphere0 = dynamic_cast<CColliderSphere*>(c0);
		CVector sp0;
		float sr0;
		sphere0->Get(&sp0, &sr0);

		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionSphereLine(sp0, sr0, ls, le, hit, true);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere1 = dynamic_cast<CColliderSphere*>(c1);
			CVector sp1;
			float sr1;
			sphere1->Get(&sp1, &sr1);
			return CollisionSphere(sp0, sr0, sp1, sr1, hit);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleSphere(t0, t1, t2, sp0, sr0, hit, false);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs, ce;
			capsule->Get(&cs, &ce);
			float cr = capsule->Radius();
			return CollisionSphereCapsule(sp0, sr0, cs, ce, cr, hit, true);
		}
		break;
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshSpehre(tris, sphere0, hit, false);
		}
		}
		break;
	}
	case EColliderType::eTriangle:
	{
		CColliderTriangle* triangle0 = dynamic_cast<CColliderTriangle*>(c0);
		CVector t00, t01, t02;
		triangle0->Get(&t00, &t01, &t02);
		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionTriangleLine(t00, t01, t02, ls, le, hit, true);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionTriangleSphere(t00, t01, t02, sp, sr, hit, true);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle1 = dynamic_cast<CColliderTriangle*>(c1);
			CVector t10, t11, t12;
			triangle1->Get(&t10, &t11, &t12);
			return CollisionTriangle(t00, t01, t02, t10, t11, t12, hit);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs, ce;
			capsule->Get(&cs, &ce);
			float cr = capsule->Radius();
			return CollisionTriangleCapsule(t00, t01, t02, cs, ce, cr, hit, true);
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshTriangle(tris, triangle0, hit, false);
		}
		}
		break;
	}
	case EColliderType::eCapsule:
	{
		CColliderCapsule* capsule0 = dynamic_cast<CColliderCapsule*>(c0);
		CVector cs0, ce0;
		capsule0->Get(&cs0, &ce0);
		float cr0 = capsule0->Radius();

		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line1 = dynamic_cast<CColliderLine*>(c1);
			CVector ls1, le1;
			line1->Get(&ls1, &le1);
			return CollisionCapsuleLine(cs0, ce0, cr0, ls1, le1, hit, false);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionSphereCapsule(sp, sr, cs0, ce0, cr0, hit, false);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleCapsule(t0, t1, t2, cs0, ce0, cr0, hit, false);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule1 = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs1, ce1;
			capsule1->Get(&cs1, &ce1);
			float cr1 = capsule1->Radius();
			return CollisionCapsule(cs0, ce0, cr0, cs1, ce1, cr1, hit);
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshCapsule(tris, capsule0, hit, false);
		}
		}
		break;
	}
	case EColliderType::eMesh:
	{
		CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c0);
		auto tris = mesh->Get();
		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionMeshLine(tris, ls, le, line->Bounds(), hit, true);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			return CollisionMeshSpehre(tris, sphere, hit, true);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			return CollisionMeshTriangle(tris, triangle, hit, true);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			return CollisionMeshCapsule(tris, capsule, hit, true);
		}
		case EColliderType::eMesh:
		{
			// ���b�V���ƃ��b�V���͏Փ˔�����s��Ȃ�
			return false;
		}
		}
		break;
	}
	}
	return false;
}

// ���C�ƃR���C�_�[�̏Փ˔���
bool CCollider::CollisionRay(CCollider* c, const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �R���C�_�[��null�Ȃ�΁A�Փ˂��Ă��Ȃ�
	if (c == nullptr) return false;
	// ���C�̒�����0�Ȃ�΁A�Փ˂��Ă��Ȃ�
	CVector v = end - start;
	if (v.LengthSqr() == 0.0f) return false;

	// �R���C�_�[�̎�ނɂ���ďՓ˔����؂�ւ���
	switch(c->Type())
	{
		// ���R���C�_�[�Ƃ̏Փ�
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionLine(start, end, ls, le, hit);
		}
		// ���R���C�_�[�Ƃ̏Փ�
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionSphereLine(sp, sr, start, end, hit, false);
		}
		// �O�p�`�R���C�_�[�Ƃ̏Փ�
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleLine(t0, t1, t2, start, end, hit, false);
		}
		// ���b�V���R���C�_�[�Ƃ̏Փ�
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c);
			auto tris = mesh->Get();
			CBounds bounds = CBounds::GetLineBounds(start, end);
			return CollisionMeshLine(tris, start, end, bounds, hit, false);
		}
	}

	// ����ȊO�͎��s
	return false;
}

// �Փˎ��̉����߂��������Z�o
float CCollider::CalcPushBackRatio(CCollider* self, CCollider* other)
{
	// ���g�̃R���C�_�[�������߂��̉e�����󂯂Ȃ�
	if (self->IsKinematic()) return 0.0f;
	// ����̃R���C�_�[�������߂��̉e�����󂯂Ȃ�
	if (other->IsKinematic()) return 1.0f;

	// �����̃R���C�_�[�������߂��̉e�����󂯂�ꍇ�́A
	// �����̃R���C�_�[�̏d�ʂŉ����߂��������Z�o
	float sw = self->GetWeight();
	float ow = other->GetWeight();
	// �����d�ʂȂ�΁A50%�̉e�����󂯂�
	if (sw == ow) return 0.5f;
	// ���g�̏d�ʂ�0�Ȃ�΁A100%�̉e�����󂯂�
	if (sw <= 0.0f) return 1.0f;
	// ����̏d�ʂ�0�Ȃ�΁A�e���͎󂯂Ȃ�
	if (ow <= 0.0f) return 0.0f;

	// �d�ʂ̊������v�Z���ĕԂ�
	return 1.0f - Math::Clamp01(sw / (sw + ow));
}
