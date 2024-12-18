//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CSword.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Player\\anim\\"
#define BODY_HEIGHT 16.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f	// 本体のコライダーの幅
#define MOVE_SPEED 0.75f	// 移動速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

#define MOTION_BLUR_TIME 3.0f	// モーションブラーを掛ける時間
#define MOTION_BLUR_WIDTH 1.0f	// モーションブラーの幅
#define MOTION_BLUR_COUNT 5		// モーションブラーの反復回数

#define ATTACK_START_FRAME 26.0f	// 斬り攻撃の開始フレーム
#define ATTACK_END_FRAME 50.0f		// 斬り攻撃の終了フレーム
// 斬り攻撃の剣のオフセット座標
#define ATTACK_SWORD_OFFSET_POS CVector(0.0f, 7.2f, 3.5f)
// 斬り攻撃の剣のオフセット向き
#define ATTACK_SWORD_OFFSET_ROT CVector(-20.0f, 0.0f, -7.0f)

#define ATTACK2_START_FRAME 26.0f	// 蹴り攻撃の開始フレーム
#define ATTACK2_END_FRAME 40.0f		// 蹴り攻撃の終了フレーム
#define ATTACK2_COL_RADIUS 7.5f		// 蹴り攻撃のコライダーの半径
// 蹴り攻撃のコライダーのオフセット座標
#define ATTACK2_COL_OFFSET_POS CVector(0.0f, 4.0f, 5.0f)

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	153.0f,	1.0f	},	// 待機
	{ ANIM_PATH"walk.x",		true,	66.0f,	1.0f	},	// 歩行
	{ ANIM_PATH"attack.x",		false,	92.0f,	1.0f	},	// 斬り攻撃
	{ ANIM_PATH"kick.x",		false,	74.0f,	1.0f	},	// 蹴り攻撃
	{ ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// ジャンプ開始
	{ ANIM_PATH"jump.x",		true,	1.0f,	1.0f	},	// ジャンプ中
	{ ANIM_PATH"jump_end.x",	false,	26.0f,	1.0f	},	// ジャンプ終了
	{ ANIM_PATH"hit.x",			false,	44.0f,	1.0f	},	// 仰け反り
};

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mMotionBlurRemainTime(0.0f)
	, mpSword(nullptr)
{
	mMaxHp = 100000;
	mHp = mMaxHp;

	//インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Player");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject, ETag::eEnemy });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy, ELayer::eAttackCol });

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f),
		CQuaternion(0.0f, 90.0f, 0.0f).Matrix()
	);

	// プレイヤーの剣を作成
	mpSword = new CSword
	(
		this,
		ETag::ePlayer,
		{ ETag::eEnemy },	// 敵のタグが設定されたコライダーと衝突
		{ ELayer::eEnemy }	// 敵のレイヤーが設定されたコライダーと衝突
	);

	// 右手のフレームを取得し、
	// 剣にプレイヤーの右手の行列をアタッチ
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	mpSword->SetAttachMtx(&frame->CombinedMatrix());
	mpSword->Position(ATTACK_SWORD_OFFSET_POS);
	mpSword->Rotation(ATTACK_SWORD_OFFSET_ROT);

	// 蹴り攻撃用のコライダーを作成
	mpAttack2Col = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		ATTACK2_COL_RADIUS
	);
	// 敵の本体のコライダーとのみヒットするように設定
	mpAttack2Col->SetCollisionTags({ ETag::eEnemy });
	mpAttack2Col->SetCollisionLayers({ ELayer::eEnemy });
	// プレイヤーの正面にズラす
	mpAttack2Col->Position(ATTACK2_COL_OFFSET_POS);
	// 攻撃コライダーは最初はオフにしておく
	mpAttack2Col->SetEnable(false);
}

CPlayer::~CPlayer()
{
	// コライダーを削除
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpAttack2Col);

	// 剣が存在したら、
	if (mpSword != nullptr)
	{
		// 持ち主を解除してから、削除
		mpSword->SetOwner(nullptr);
		mpSword->Kill();
	}
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type, bool restart)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, restart);
}

// 状態を切り替え
void CPlayer::ChangeState(EState state)
{
	if (mState == state) return;

	// 攻撃中に他に状態に変わる時は、
	// 攻撃終了処理を呼び出しておく
	if (IsAttacking())
	{
		AttackEnd();
	}

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// 待機
void CPlayer::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		// 左クリックで斬撃攻撃へ移行
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack1);
		}
		// 右クリックでキック攻撃へ以降
		else if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack2);
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			ChangeState(EState::eJumpStart);
		}
	}
}

// 斬り攻撃
void CPlayer::UpdateAttack1()
{
	switch (mStateStep)
	{
		case 0:
			// 攻撃アニメーションを開始
			ChangeAnimation(EAnimType::eAttack, true);
			// 斬撃SEの再生済みフラグを初期化
			mIsPlayedSlashSE = false;
			// 斬撃エフェクトの生成済みフラグを初期化
			mIsSpawnedSlashEffect = false;

			mStateStep++;
			break;
		case 1:
			if (GetAnimationFrame() >= ATTACK_START_FRAME)
			{
				// 斬撃SEを再生
				mpSlashSE->Play();
				// 攻撃開始
				AttackStart();

				mStateStep++;
			}
			break;
		case 2:
			if (GetAnimationFrame() >= ATTACK_END_FRAME)
			{
				// 攻撃終了
				AttackEnd();

				mStateStep++;
			}
			break;
		case 3:
			// 攻撃アニメーションが終了したら、
			if (IsAnimationFinished())
			{
				// 待機状態へ移行
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			break;
	}
}

// 蹴り攻撃
void CPlayer::UpdateAttack2()
{
	switch (mStateStep)
	{
		case 0:
			// 攻撃アニメーションを開始
			ChangeAnimation(EAnimType::eKick, true);
			mStateStep++;
			break;
		case 1:
			if (GetAnimationFrame() >= ATTACK2_START_FRAME)
			{
				AttackStart();
				mStateStep++;
			}
			break;
		case 2:
			if (GetAnimationFrame() >= ATTACK2_END_FRAME)
			{
				AttackEnd();
				mStateStep++;
			}
			break;
		case 3:
			// 攻撃アニメーションが終了したら、
			if (IsAnimationFinished())
			{
				// 待機状態へ移行
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			break;
	}
}

// ジャンプ開始
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	ChangeState(EState::eJump);

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// ジャンプ中
void CPlayer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		ChangeState(EState::eJumpEnd);
	}
}

// ジャンプ終了
void CPlayer::UpdateJumpEnd()
{
	// ジャンプアニメーションが終了かつ、
	// 地面に接地したら、待機状態へ戻す
	if (IsAnimationFinished() && mIsGrounded)
	{
		ChangeState(EState::eIdle);
	}
}

// 仰け反り
void CPlayer::UpdateHit()
{
	switch (mStateStep)
	{
		case 0:
			// 仰け反りアニメーションを開始
			ChangeAnimation(EAnimType::eHit, true);
			mStateStep++;
			break;
		case 1:
			// 仰け反りアニメーションが終了したら
			if (IsAnimationFinished())
			{
				// 待機状態へ移行
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			break;
	}
}

// オブジェクト削除を伝える
void CPlayer::DeleteObject(CObjectBase* obj)
{
	// 剣が先に削除されたら、剣のポインタを初期化
	if (mpSword == obj)
	{
		mpSword = nullptr;
	}
}

// キーの入力情報から移動ベクトルを求める
CVector CPlayer::CalcMoveVec() const
{
	CVector move = CVector::zero;

	// キーの入力ベクトルを取得
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// 入力ベクトルの長さで入力されているか判定
	if (input.LengthSqr() > 0.0f)
	{
		// 上方向ベクトル(設置している場合は、地面の法線)
		CVector up = mIsGrounded ? mGroundNormal : CVector::up;
		// カメラの向きに合わせた移動ベクトルに変換
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		camForward.Y(0.0f);
		camForward.Normalize();
		// カメラの正面方向ベクトルと上方向ベクトルの外積から
		// 横方向の移動ベクトルを求める
		CVector moveSide = CVector::Cross(up, camForward);
		// 横方向の移動ベクトルと上方向ベクトルの外積から
		// 正面方向の移動ベクトルを求める
		CVector moveForward = CVector::Cross(moveSide, up);

		// 求めた各方向の移動ベクトルから、
		// 最終的なプレイヤーの移動ベクトルを求める
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}

// 移動の更新処理
void CPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED;

		// 待機状態であれば、歩行アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eWalk);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eIdle);
		}
	}
}

// モーションブラーの更新処理
void CPlayer::UpdateMotionBlur()
{
	// モーションブラーの残り時間が残っていなければ、処理しない
	if (mMotionBlurRemainTime <= 0.0f) return;
	// 現在のカメラを取得し、存在しなければ処理しない
	CCamera* currentCamera = CCamera::CurrentCamera();
	if (currentCamera == nullptr) return;

	// カメラの向きと反対方向へブラーを掛けるため、
	// 反転したカメラの向きを取得
	CVector camDir = -currentCamera->VectorZ();

	// 残り時間から経過時間の割合を取得（経過時間の割合 = 1 - 残り時間の割合）
	float percent = 1.0f - mMotionBlurRemainTime / MOTION_BLUR_TIME;
	// ブラーの幅をサインカーブで経過時間に合わせて増減させる
	float ratio = sinf(M_PI * percent);
	float width = MOTION_BLUR_WIDTH * ratio;

	// モーションブラーのパラメータを設定
	System::SetMotionBlur(camDir, width, MOTION_BLUR_COUNT);

	// 残り時間を経過時間分減少させる
	mMotionBlurRemainTime -= Times::DeltaTime();
	// 残り時間がなくなれば、
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// モーションブラーをオフにする
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
	}
}

// 更新
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
		case EState::eIdle:			UpdateIdle();		break;
		// 斬り攻撃
		case EState::eAttack1:		UpdateAttack1();	break;
		// 蹴り攻撃
		case EState::eAttack2:		UpdateAttack2();	break;
		// ジャンプ開始
		case EState::eJumpStart:	UpdateJumpStart();	break;
		// ジャンプ中
		case EState::eJump:			UpdateJump();		break;
		// ジャンプ終了
		case EState::eJumpEnd:		UpdateJumpEnd();	break;
		// 仰け反り
		case EState::eHit:			UpdateHit();		break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	// プレイヤーを移動方向へ向ける
	CVector current = VectorZ();
	CVector target = moveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// ホイールクリックで弾丸発射
	if (CInput::PushKey(VK_MBUTTON))
	{
		// 弾丸を生成
		new CBullet
		(
			// 発射位置
			Position() + CVector(0.0f, 10.0f, 0.0f) + VectorZ() * 20.0f,
			VectorZ(),	// 発射方向
			1000.0f,	// 移動距離
			1000.0f		// 飛距離
		);
	}

	// 「E」キーで炎の発射をオンオフする
	if (CInput::PushKey('E'))
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
		else
		{
			mpFlamethrower->Stop();
		}
	}

	// 「P」キーを押したら、ゲームを終了
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// 「B」キーを押したら、モーションブラー開始
	if (CInput::PushKey('B'))
	{
		// モーションブラーを掛けている最中であれば、
		// 新しくモーションブラーを掛け直さない
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
		}
	}

	// モーションブラーの更新処理
	UpdateMotionBlur();

	// キャラクターの更新
	CXCharacter::Update();

	// 武器の行列を更新
	mpSword->UpdateMtx();

	CVector pos = Position();
	CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("PlayerState:%d\n", mState);

	mIsGrounded = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// 攻撃中か
bool CPlayer::IsAttacking() const
{
	// 斬り攻撃中
	if (mState == EState::eAttack1) return true;
	// 蹴り攻撃中
	if (mState == EState::eAttack2) return true;

	// 攻撃中でない
	return false;
}

// 攻撃開始
void CPlayer::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CXCharacter::AttackStart();

	// 斬り攻撃中であれば、剣のコライダーをオンにする
	if (mState == EState::eAttack1)
	{
		mpSword->SetEnableCol(true);
	}
	// 蹴り攻撃中であれば、蹴り攻撃用のコライダーをオンにする
	else if (mState == EState::eAttack2)
	{
		mpAttack2Col->SetEnable(true);
	}
}

// 攻撃終了
void CPlayer::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();

	// 攻撃コライダーをオフ
	mpSword->SetEnableCol(false);
	mpAttack2Col->SetEnable(false);
}

// ダメージを受ける
void CPlayer::TakeDamage(int damage, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CXCharacter::TakeDamage(damage, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		// 仰け反り状態へ移行
		ChangeState(EState::eHit);

		// 攻撃を加えた相手の方向へ向く
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
}

// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体のコライダーの衝突判定
	if (self == mpBodyCol)
	{
		// フィールドとの衝突
		if (other->Layer() == ELayer::eField)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、床と衝突した
			if (dot >= 0.0f)
			{
				// 落下などで床に上から衝突した時（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// 内積の結果がマイナスであれば、天井と衝突した
			else
			{
				// ジャンプなどで天井に下から衝突した時（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// 敵と衝突した場合
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
	// 剣のコライダーが衝突した
	else if (self == mpSword->Collider())
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			AddAttackHitObj(hitChara);
			hitChara->TakeDamage(1, this);
		}
	}
	// 蹴り攻撃のコライダーが衝突した
	else if (self == mpAttack2Col)
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			AddAttackHitObj(hitChara);
			hitChara->TakeDamage(1, this);
		}
	}
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}
