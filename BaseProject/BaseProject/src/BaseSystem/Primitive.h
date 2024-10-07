#pragma once
#include "CVector.h"
#include "CColor.h"

// ��{�}�`�̕`��֘A
namespace Primitive
{
	/// <summary>
	/// ������`��i3D�j
	/// </summary>
	/// <param name="s">�n�_</param>
	/// <param name="e">�I�_</param>
	/// <param name="color">�F</param>
	/// <param name="lineWidth">���̑���</param>
	void DrawLine(const CVector& s, const CVector& e, const CColor& color, float lineWidth = 1.0f);
	/// <summary>
	/// ������`��i2D�j
	/// </summary>
	/// <param name="s">�n�_</param>
	/// <param name="e">�I�_</param>
	/// <param name="color">�F</param>
	/// <param name="lineWidth">���̑���</param>
	void DrawLine2D(const CVector2& s, const CVector2& e, const CColor& color, float lineWidth = 1.0f);

	/// <summary>
	/// ����`��
	/// </summary>
	/// <param name="pos">�`��s��</param>
	/// <param name="rad">���a</param>
	/// <param name="color">�F</param>
	void DrawSphere(const CMatrix& m, float rad, const CColor& color);
	/// <summary>
	/// ���C���[�t���[���̋���`��
	/// </summary>
	/// <param name="pos">�`��s��</param>
	/// <param name="rad">���a</param>
	/// <param name="color">�F</param>
	void DrawWireSphere(const CMatrix& m, float rad, const CColor& color);

	/// <summary>
	/// �O�p�`��`��
	/// </summary>
	/// <param name="v0">1�ڂ̒��_</param>
	/// <param name="v1">2�ڂ̒��_</param>
	/// <param name="v2">3�ڂ̒��_</param>
	/// <param name="color">�F</param>
	void DrawTriangle(const CVector& v0, const CVector& v1, const CVector& v2, const CColor& color);

	/// <summary>
	/// �|���S����`��
	/// </summary>
	/// <param name="m">�`��s��</param>
	/// <param name="size">�c���̃T�C�Y</param>
	/// <param name="color">�F</param>
	void DrawQuad(const CMatrix& m, const CVector2& size, const CColor& color);

	/// <summary>
	/// �J�v�Z����`��
	/// </summary>
	/// <param name="sp">�J�v�Z�����\����������̎n�_</param>
	/// <param name="ep">�J�v�Z�����\����������̏I�_</param>
	/// <param name="rad">�J�v�Z���̔��a</param>
	/// <param name="color">�F</param>
	void DrawCapsule(const CVector& sp, const CVector& ep, float rad, const CColor& color);

	/// <summary>
	/// ���C���[�t���[���̃J�v�Z����`��
	/// </summary>
	/// <param name="sp">�J�v�Z�����\����������̎n�_</param>
	/// <param name="ep">�J�v�Z�����\����������̏I�_</param>
	/// <param name="rad">�J�v�Z���̔��a</param>
	/// <param name="color">�F</param>
	void DrawWireCapsule(const CVector& sp, const CVector& ep, float rad, const CColor& color);
};