#pragma once

namespace Time
{
	// �ڕW�t���[�����[�g���擾
	int TargetFPS();
	// �v�Z��ł�1�t���[���̌o�ߎ��Ԃ��擾
	float CalcDeltaTime();
	// �O��̃t���[����FPS���擾
	float FPS();
	// �O��̃t���[���̌o�ߎ��Ԃ��擾
	float DeltaTime();
}