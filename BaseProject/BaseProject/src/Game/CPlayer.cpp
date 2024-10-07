//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// Tポーズ
	{ "Character\\Player\\anim\\idle.x",		true,	153.0f	},	// 待機
	{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// 歩行
	{ "Character\\Player\\anim\\attack.x",		false,	91.0f	},	// 攻撃
	{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// ジャンプ開始
	{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// ジャンプ中
	{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// ジャンプ終了
};

#define PLAYER_HEIGHT 16.0f
#define MOVE_SPEED 0.375f * 2.0f
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

// モーションブラーを掛ける時間
#define MOTION_BLUR_TIME 3.0f
// モーションブラーの幅
#define MOTION_BLUR_WIDTH 1.0f
// モーションブラーの反復回数
#define MOTION_BLUR_COUNT 5

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mMotionBlurRemainTime(0.0f)
{
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

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f),
		CQuaternion(0.0f, 90.0f, 0.0f).Matrix()
	);
}

CPlayer::~CPlayer()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機
void CPlayer::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		// 左クリックで攻撃状態へ移行
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			mState = EState::eAttack;
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
	}
}

// 攻撃
void CPlayer::UpdateAttack()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack);
	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;

	// 斬撃SEの再生済みフラグを初期化
	mIsPlayedSlashSE = false;
	// 斬撃エフェクトの生成済みフラグを初期化
	mIsSpawnedSlashEffect = false;
}

// 攻撃終了待ち
void CPlayer::UpdateAttackWait()
{
	// 斬撃SEを再生していないかつ、アニメーションが25%以上進行したら、
	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
	{
		// 斬撃SEを再生
		mpSlashSE->Play();
		mIsPlayedSlashSE = true;
	}

	// 斬撃エフェクトを生成していないかつ、アニメーションが35%以上進行したら、
	if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.35f)
	{
		// 斬撃エフェクトを生成して、正面方向へ飛ばす
		CSlash* slash = new CSlash
		(
			this,
			Position() + CVector(0.0f, 10.0f, 0.0f),
			VectorZ(),
			300.0f,
			100.0f
		);
		// 斬撃エフェクトの色設定
		slash->SetColor(CColor(0.15f, 0.5f, 0.5f));

		mIsSpawnedSlashEffect = true;
	}

	// 攻撃アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// ジャンプ開始
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	mState = EState::eJump;

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// ジャンプ中
void CPlayer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// ジャンプ終了
void CPlayer::UpdateJumpEnd()
{
	// ジャンプアニメーションが終了かつ、
	// 地面に接地したら、待機状態へ戻す
	if (IsAnimationFinished() && mIsGrounded)
	{
		mState = EState::eIdle;
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
	mMotionBlurRemainTime -= Time::DeltaTime();
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
		case EState::eIdle:
			UpdateIdle();
			break;
		// 攻撃
		case EState::eAttack:
			UpdateAttack();
			break;
		// 攻撃終了待ち
		case EState::eAttackWait:
			UpdateAttackWait();
			break;
		// ジャンプ開始
		case EState::eJumpStart:
			UpdateJumpStart();
			break;
		// ジャンプ中
		case EState::eJump:
			UpdateJump();
			break;
		// ジャンプ終了
		case EState::eJumpEnd:
			UpdateJumpEnd();
			break;
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

	// 右クリックで弾丸発射
	if (CInput::PushKey(VK_RBUTTON))
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

	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("State:%d\n", mState);

	mIsGrounded = false;

	CDebugPrint::Print("FPS:%f\n", Time::FPS());
}

// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
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
	}
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}
