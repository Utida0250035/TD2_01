#include "Player.h"

#include <algorithm>
#include "Math/Lerp.h"
#include "System/EntityFactory.h"
#include "Particle/CommandParticle.h"
#include "ParticleExplosionRing.h"
#include "ParticleBigExplosion.h"
#include "Floor.h"

#include "KeMatrix3D.h"
#include "KeMainCamera.h"
#include "Time/DeltaTime.h"

#include "SEPlayer.h"

using namespace Atrum;
using namespace Atrum::Math;

void Player::Initialize() {

	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::PLAYER);
	entity_->SetInitialTransform({{1.0f,1.0f,1.0f}, {}, {-20.0f, -20.0f, 0.0f}});

	entity_->AddUpdCmp<CmpLhRigidBody>();
	entity_->AddUpdCmp<CmpHitSphere>();
	entity_->AddUpdCmp<CmpHealthHit>();
	entity_->AddUpdCmp<CmpPlayer>();
	entity_->AddDrawCmp<CmpMesh>();

	CmpPlayer* cmpPlayer = entity_->GetUpdCmp<CmpPlayer>();

	cmpPlayer->SetAnotherOwner(this);

	CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("player"));

	CmpLhRigidBody* cmpLhRigidBody = entity_->GetUpdCmp<CmpLhRigidBody>();
	cmpLhRigidBody->SetGravity({0.0f, -gravity_, 0.0f});
	cmpLhRigidBody->SetIsUseSpaceEnd(false);
	cmpLhRigidBody->SetIsAutoMove(false);

	CmpHitSphere* cmpHitSphere = entity_->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere->SetSpheres({
	    {{}, 2.0f}
    });

	cmpHealthHit_ = entity_->GetUpdCmp<CmpHealthHit>();
	std::vector<EntityType> hurterTypes;
	cmpHealthHit_->SetInitialHealthCount(maxHealth_);
	cmpHealthHit_->SetInvincibleTime(invincibleTime_);
	hurterTypes.push_back(EntityType::ENEMY_WEAPON);
	cmpHealthHit_->SetHurterTypes(hurterTypes);

	entity_->ResolveDependences();
	entity_->Initialize();

	gaugeSprite_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("white1x1.png"), {16.0f, 64.0f});

	//-------------------------------------------
	// 矢印方向表示

	directionArrowModel_ = KamataEngine::Model::CreateFromOBJ("directionArrow", false);
	arrowTransform_.Initialize();
	arrowTransform_.translation_.x = entity_.get()->RefTransform().translate.x;
	arrowTransform_.translation_.y = entity_.get()->RefTransform().translate.y;
	arrowTransform_.translation_.z = entity_.get()->RefTransform().translate.z;
	arrowTransform_.rotation_.z = std::atan2(1.0f, 0.0f);

	arrowTransform_.matWorld_ = MakeWorldMatrix(arrowTransform_.translation_, arrowTransform_.scale_, arrowTransform_.rotation_);
	arrowTransform_.TransferMatrix();

	//
	//-------------------------------------------

	// 体力画像 (32x32)
	healthTexture_ = KamataEngine::TextureManager::Load("white1x1.png");

	for (uint32_t i = 0; i < maxHealth_; i++) {

		healthSprites_[i] = KamataEngine::Sprite::Create(healthTexture_, {16.0f + (36.0f * i), 16.0f}, {1.0f, 1.0f, 0.0f, 1.0f});
		healthSprites_[i]->SetSize({32.0f, 32.0f});
	
	}

	inputManual_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("inputManual.png"), {16.0f, 704.0f});
	inputManual_->SetAnchorPoint({0.0f, 1.0f});


}

void Player::AddCharge() {

	superCharge_++;

	if (superCharge_ > maxCharge_) {
		superCharge_ = maxCharge_;
	}

	std::cout << superCharge_ << std::endl;

}

void Player::SetArrowDireciton(const Vector2 direction) {

	float angle = std::atan2(direction.y, direction.x);

	if (direction.x == 0.0f && direction.y == 0.0f) {
		angle = std::atan2(1.0f, 0.0f);
	}

	arrowTransform_.rotation_.z = angle;

}

void Player::Update() {

	// 演出による一時停止
	if (CommandParticle::GetInstance()->IsExist("bossAppear") || CommandParticle::GetInstance()->IsExist("bossDefeat")) {

		if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			SEPlayer::GetInstance()->Play("./Resources/seEnemyAppear.mp3");
		}

		return;
	}

	if (onGround_) {
		currentBomb_ = bombMax_;
	}

	// 死亡終了確認
	if (isDeath_) {
		deathTimer_ += FrameDeltaTime::GetInstance()->GetDeltaTime();

		if (deathTimer_ >= deathTime_) {
			isDeathFinished_ = true;
		}

		deathVelocity_ += -180.0f * FrameDeltaTime::GetInstance()->GetDeltaTime();
		entity_->RefTransform().translate.y += deathVelocity_ * FrameDeltaTime::GetInstance()->GetDeltaTime();

		return;

	}

	uint32_t preHealth = cmpHealthHit_->GetHealthCount();

	entity_->Update();

	uint32_t afterHealth = cmpHealthHit_->GetHealthCount();

	if (cmpHealthHit_->GetHealthCount() == 0 || entity_->GetWorldPosition().y < -30.0f) {

		isDeath_ = true;

		CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
		cmpMesh->SetColor({0.0f, 0.0f, 0.0f, 1.0f});

	}

	// 被弾検知
	if (!isInvincible_) {
		if (afterHealth != preHealth) {

			currentHealth_--;
			isInvincible_ = true;

			if (cmpHealthHit_->GetHealthCount() == 0) {

				isDeath_ = true;

				deathVelocity_ = 70.0f;
				CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
				cmpMesh->SetColor({0.0f, 0.0f, 0.0f, 1.0f});


			} else {

				invincibleTimer_ = invincibleTime_;

				CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
				cmpMesh->SetColor({1.0f, 0.0f, 0.0f, 1.0f});

			}

		}
	}

	// 無敵クールダウン
	if (!isDeath_) {
		if (isInvincible_) {

			invincibleTimer_ -= 1.0f * FrameDeltaTime::GetInstance()->GetDeltaTime();

			if (invincibleTimer_ <= 0.0f) {

				isInvincible_ = false;

				CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
				cmpMesh->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
			}
		}
	}

	arrowTransform_.translation_.x = entity_.get()->RefTransform().translate.x;
	arrowTransform_.translation_.y = entity_.get()->RefTransform().translate.y;
	arrowTransform_.translation_.z = entity_.get()->RefTransform().translate.z;

	arrowTransform_.matWorld_ = MakeWorldMatrix(arrowTransform_.translation_, arrowTransform_.scale_, arrowTransform_.rotation_);
	arrowTransform_.TransferMatrix();

	for (size_t i = 0; i < booms_.size(); ++i) {

		if (!booms_[i])
			continue;

		booms_[i]->Update();

		if (booms_[i]->GetState() == Entity::State::Sleep) {
		
			booms_[i]->Finalize();

			// 寿命を終えた爆発エンティティを削除
			booms_.erase(booms_.begin() + i);

			// 削除した分デクリメントする
			--i;
		
		}

	}

	// UI
	gaugeSprite_->SetSize({(static_cast<float>(superCharge_) / static_cast<float>(maxCharge_)) * chargeUIMax_, 32.0f});

}

bool Player::CanExplode() {

	// 爆発可能回数が0でないかを確認する
	if (currentBomb_ > 0) {
		return true;
	}

	return false;

}

void Player::Draw() {

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	for (auto& i : booms_) {
	
		i->Draw();
	
	}

	KamataEngine::Model::PreDraw(KamataEngine::ModelCommon::CullingMode::kBack, KamataEngine::Model::BlendMode::kNormal, KamataEngine::Model::DepthTestMode::kReadOnly);

	directionArrowModel_->Draw(arrowTransform_, *KeMainCamera::GetInstance()->camera_);

	KamataEngine::Model::PostDraw();

	CommandParticle::GetInstance()->Draw("explosion");
	CommandParticle::GetInstance()->Draw("bigExplosion");

	KamataEngine::Model::PostDraw();
}

void Player::DrawUI() {

	KamataEngine::Sprite::PreDraw();

	gaugeSprite_->Draw();

	for (uint32_t i = 0; i < currentHealth_; i++) {
		healthSprites_[i]->Draw();
	}

	inputManual_->Draw();

	KamataEngine::Sprite::PostDraw();

}

namespace Atrum {

	void CmpPlayer::ResolveDependence() {

		rigidBody_ = RefOwner().GetUpdCmp<CmpLhRigidBody>();

}
	void CmpPlayer::Initialize() {

		explosionFireModel_ = KamataEngine::Model::CreateFromOBJ("explosionFire", false);
		explosionSmokeModel_ = KamataEngine::Model::CreateFromOBJ("explosionSmoke", false);
		explosionRingModel_ = KamataEngine::Model::CreateFromOBJ("explosionRing", false);

	}

	void CmpPlayer::Update() {

		// 衝突情報の初期化
	    Player::CollisionMapInfo collisionMapInfo{};
	
		//--------------------------
		// 無重力時間

		noGravityTime_ -= 1.0f * FrameDeltaTime::GetInstance()->GetDeltaTime();

	    if (noGravityTime_ <= 0.0f) {
		    rigidBody_->SetIsUseGravity(true);
		}

		//
	    //--------------------------

		//--------------------------
		// ベクトルと座標更新

		//--------------
		// 変数

		#pragma region var

		// ベクトル
		Vector3& velocity = rigidBody_->RefVelocity();

		// トランスフォーム
		TransformLH &transform = RefOwner().RefTransform();

		// 移動方向
	    Vector2 moveDirection = {};

		KamataEngine::Input* input_ = KamataEngine::Input::GetInstance();

		#pragma endregion

		//
	    //--------------

		//--------------
		//

		#pragma region update

		// 移動制限
	    if (transform.translate.x >= xVectorMoveLimit_) {
		    transform.translate.x = xVectorMoveLimit_;
		}

		if (transform.translate.x <= -xVectorMoveLimit_) {
		    transform.translate.x = -xVectorMoveLimit_;
		}

		if (transform.translate.y >= upperMoveLimit_) {
		    transform.translate.y = upperMoveLimit_;
		}

		// 減衰
		velocity = Atrum::Interpolation::Lerp(velocity, {}, 0.01f);

		#pragma endregion

		//
	    //--------------

		//
	    //--------------------------

		//--------------------------
		// 方向指定

		#pragma region directionInput

	    if (input_->PushKey(DIK_W) || input_->PushKey(DIK_UP)) {
		    moveDirection.y += 1.0f;
	    }

	    if (input_->PushKey(DIK_S) || input_->PushKey(DIK_DOWN)) {
		    moveDirection.y -= 1.0f;
	    }

	    if (input_->PushKey(DIK_A) || input_->PushKey(DIK_LEFT)) {
		    moveDirection.x -= 1.0f;
	    }

	    if (input_->PushKey(DIK_D) || input_->PushKey(DIK_RIGHT)) {
		    moveDirection.x += 1.0f;
	    }

		anotherOwner_->SetArrowDireciton(moveDirection);

		#pragma endregion

		//
	    //--------------------------

		//--------------------------
		// 爆発

	    if (input_->TriggerKey(DIK_SPACE)) {

			if (!anotherOwner_->CanExplode()) {

					SEPlayer::GetInstance()->Play("./Resources/seEnemyAppear.mp3");

			    return;
			}

			anotherOwner_->SetOnGround(false);
			anotherOwner_->ExplodeBombRemain();

		    // 無重力化
		    noGravityTime_ = noGravityTimer_;
		    rigidBody_->SetIsUseGravity(false);

		    // 方向未指定の場合は真上に指定
		    if (moveDirection.x == 0.0f && moveDirection.y == 0.0f) {
			    moveDirection.y = 1.0f;
		    }

		    // ベクトル更新
		    velocity.x = moveDirection.x * Player::jumpPower_;
		    velocity.y = moveDirection.y * Player::jumpPower_;

		    // 判定エンティティ
		    anotherOwner_->RefBooms().emplace_back(Atrum::EntityFactory::Boom(RefOwner().GetWorldPosition(), anotherOwner_));

		    ParticleExplosion* particle = new ParticleExplosion;
		    particle->SetInitialTranslation(RefOwner().GetWorldPosition());
		    particle->SetModel(explosionFireModel_, explosionSmokeModel_, explosionRingModel_);
		    particle->Initialize();

			CommandParticle::GetInstance()->Generate("explosion", particle);

	    }

		// 移動量に速度の値をコピー
	    float dt = FrameDeltaTime::GetInstance()->GetDeltaTime();
	    collisionMapInfo.moveAmount = velocity * dt;

		// マップ衝突チェック
	    CollisionMap(collisionMapInfo);

	    // 移動
	    Move(collisionMapInfo);

	    // 天井
	    HitCeilingUpdate(collisionMapInfo);
	    // 壁
	    HitWallUpdate(collisionMapInfo);
	    // 床
	    HitFloorUpdate(collisionMapInfo);

		//
	    //--------------------------

		//--------------------------
	    // 必殺技

		if (anotherOwner_->GetCharge() >= anotherOwner_->GetMaxCharge()) {
		    anotherOwner_->SetCharge(0);

			// 大爆発
		    anotherOwner_->RefBooms().emplace_back(Atrum::EntityFactory::BigBoom(RefOwner().GetWorldPosition(), anotherOwner_));

		    ParticleBigExplosion* particle = new ParticleBigExplosion;
		    particle->SetInitialTranslation(RefOwner().GetWorldPosition());
		    particle->SetModel(explosionFireModel_, explosionSmokeModel_, explosionRingModel_);
		    particle->Initialize();

		    CommandParticle::GetInstance()->Generate("bigExplosion", particle);
	    }

		//
	    //--------------------------
    }

    void CmpPlayer::CollisionMap(Player::CollisionMapInfo& info) {
	    for (int i = 0; i < static_cast<int>(Player::Direction::kNumDirection); i++) {
		    Player::Direction direction = static_cast<Player::Direction>(i);
		    CollisionMapAllDirections(info, direction);
	    }
	}

    void CmpPlayer::CollisionMapAllDirections(Player::CollisionMapInfo& info, Player::Direction& direction) {
	    // 方向ごとの設定
	    Player::Corner cornerSet[2] = {Player::Corner::kLeftTop, Player::Corner::kRightTop};
	    int sideXIndex = 0;
	    int sideYIndex = 0;

	    // それぞれの方向への移動あり?
	    switch (direction) {
	    case Player::Direction::kTop:
		    if (info.moveAmount.y <= 0) {
			    return;
		    }

		    cornerSet[0] = Player::Corner::kLeftTop;
		    cornerSet[1] = Player::Corner::kRightTop;

		    sideYIndex = 1;

		    break;
	    case Player::Direction::kBottom:
		    if (info.moveAmount.y >= 0) {
			    return;
		    }

		    cornerSet[0] = Player::Corner::kLeftBottom;
		    cornerSet[1] = Player::Corner::kRightBottom;

		    sideYIndex = -1;

		    break;
	    case Player::Direction::kRight:
		    if (info.moveAmount.x <= 0) {
			    return;
		    }

		    cornerSet[0] = Player::Corner::kRightTop;
		    cornerSet[1] = Player::Corner::kRightBottom;

		    sideXIndex = -1;

		    break;
	    case Player::Direction::kLeft:
		    if (info.moveAmount.x >= 0) {
			    return;
		    }

		    cornerSet[0] = Player::Corner::kLeftTop;
		    cornerSet[1] = Player::Corner::kLeftBottom;

		    sideXIndex = 1;

		    break;
	    }

	    // 移動後の4つの角の座標
	    std::array<Vector3, static_cast<size_t>(Player::Corner::kNumCorner)> positionsNew;

	    for (uint32_t i = 0; i < positionsNew.size(); i++) {
		    positionsNew[i] = CornerPosition(anotherOwner_->GetEntity()->GetWorldPosition() + info.moveAmount, static_cast<Player::Corner>(i));
	    }

	    Floor::MapChipType mapChipType;
	    Floor::MapChipType mapChipTypeNext;
	    Floor::IndexSet indexSet;
	    // 当たり判定
	    bool hit = false;
	    for (int i = 0; i < 2; i++) {
		    // 点の判定
		    indexSet = anotherOwner_->GetFloor()->GetMapChipIndexSetByPosition(positionsNew[static_cast<size_t>(cornerSet[i])]);
		    mapChipType = anotherOwner_->GetFloor()->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		    mapChipTypeNext = anotherOwner_->GetFloor()->GetMapChipTypeByIndex(indexSet.xIndex + sideXIndex, indexSet.yIndex + sideYIndex);
		    if (mapChipType == Floor::MapChipType::kBlock && mapChipTypeNext != Floor::MapChipType::kBlock) {
			    hit = true;
		    }
	    }

	    // ブロックにヒット?
	    if (hit) {
		    // めり込みを排除する方向に移動量を設定する
		    indexSet = anotherOwner_->GetFloor()->GetMapChipIndexSetByPosition(positionsNew[static_cast<size_t>(cornerSet[0])]);
		    // 現在座標が壁の外か判定
		    Floor::IndexSet indexSetNow;
		    indexSetNow = anotherOwner_->GetFloor()->GetMapChipIndexSetByPosition(positionsNew[static_cast<size_t>(cornerSet[0])] - info.moveAmount);
		    if (direction == Player::Direction::kTop || direction == Player::Direction::kBottom) {
			    if (indexSetNow.yIndex != indexSet.yIndex) {
				    // めり込み先ブロックの範囲矩形
				    Floor::Rect rect = anotherOwner_->GetFloor()->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
				    if (direction == Player::Direction::kTop) {
					    info.moveAmount.y = std::max(0.0f, (rect.bottom - anotherOwner_->GetEntity()->RefTransform().translate.y) - (kHeight / 2.0f + kBlank));
					    // 天井に当たったことを記録する
					    info.isHitCeiling = true;
				    } else {
					    info.moveAmount.y = std::min(0.0f, (rect.top - anotherOwner_->GetEntity()->RefTransform().translate.y) + (kHeight / 2.0f + kBlank));
					    // 床に当たったことを記録する
					    info.isHitFloor = true;
				    }
			    }
		    } else {
			    if (indexSetNow.xIndex != indexSet.xIndex) {
				    // めり込み先ブロックの範囲矩形
				    Floor::Rect rect = anotherOwner_->GetFloor()->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
				    if (direction == Player::Direction::kRight) {
					    info.moveAmount.x = std::max(0.0f, (rect.left - anotherOwner_->GetEntity()->RefTransform().translate.x) - (kWidth / 2.0f + kBlank));
				    } else {
					    info.moveAmount.x = std::min(0.0f, (rect.right - anotherOwner_->GetEntity()->RefTransform().translate.x) + (kWidth / 2.0f + kBlank));
				    }
				    // 壁に当たったことを記録する
				    info.isHitWall = true;
			    }
		    }
	    }
	}

    void CmpPlayer::Move(const Player::CollisionMapInfo& info) {
	    // 移動
	    anotherOwner_->GetEntity()->RefTransform().translate += info.moveAmount;
	}

    void CmpPlayer::HitCeilingUpdate(const Player::CollisionMapInfo& info) {
	    // 天井に当たった?
	    if (info.isHitCeiling) {
		    rigidBody_->RefVelocity().y = 0;
	    }
	}

    void CmpPlayer::HitWallUpdate(const Player::CollisionMapInfo& info) {
	    // 壁接触による減速
	    if (info.isHitWall) {
		    rigidBody_->RefVelocity().x *= (1.0f - kAttenuationWall);
	    }
	}

    void CmpPlayer::HitFloorUpdate(const Player::CollisionMapInfo& info) {
	    // 自キャラが接地状態?
	    if (anotherOwner_->GetOnGround()) {
		    // ジャンプ開始
		    if (rigidBody_->RefVelocity().y > 0.0f) {
			    anotherOwner_->SetOnGround(false);
		    } else {
			    /// 落下判定と切り替え

			    // 移動後の4つの角の座標
			    std::array<Vector3, static_cast<size_t>(Player::Corner::kNumCorner)> positionsNew;

			    for (uint32_t i = 0; i < positionsNew.size(); i++) {
				    positionsNew[i] = CornerPosition(anotherOwner_->GetEntity()->GetWorldPosition(), static_cast<Player::Corner>(i));
			    }

			    Floor::MapChipType mapChipType;
			    // 真下の当たり判定
			    bool hit = false;
			    // 左下点の判定
			    Floor::IndexSet indexSet;
			    indexSet = anotherOwner_->GetFloor()->GetMapChipIndexSetByPosition(positionsNew[static_cast<size_t>(Player::Corner::kLeftBottom)] + Vector3(0, -kBlank * 2.0f, 0));
			    mapChipType = anotherOwner_->GetFloor()->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			    if (mapChipType == Floor::MapChipType::kBlock) {
				    hit = true;
			    }
			    // 右下点の判定
			    indexSet = anotherOwner_->GetFloor()->GetMapChipIndexSetByPosition(positionsNew[static_cast<size_t>(Player::Corner::kRightBottom)] + Vector3(0, -kBlank - kBlank * 2.0f, 0));
			    mapChipType = anotherOwner_->GetFloor()->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			    if (mapChipType == Floor::MapChipType::kBlock) {
				    hit = true;
			    }

			    // 落下開始
			    if (!hit) {
				    // 空中状態に切り替え
				    anotherOwner_->SetOnGround(false);
			    }
		    }
	    } else {
		    if (info.isHitFloor) {
			    // 着地状態に切り替え
			    anotherOwner_->SetOnGround(true);
			    // 着地時に減衰
			    rigidBody_->RefVelocity() = Atrum::Interpolation::Lerp(rigidBody_->RefVelocity(), {}, 0.3f);
			    rigidBody_->RefVelocity().y = 0.0f;
		    }
	    }
	}

    Vector3 CmpPlayer::CornerPosition(const Vector3& center, Player::Corner corner) {
	    Vector3 offsetTable[static_cast<size_t>(Player::Corner::kNumCorner)] = {
	        {+kWidth / 2.0f, -kHeight / 2.0f, 0}, // kRightBottom
	        {-kWidth / 2.0f, -kHeight / 2.0f, 0}, // kLeftBottom
	        {+kWidth / 2.0f, +kHeight / 2.0f, 0}, // kRightTop
	        {-kWidth / 2.0f, +kHeight / 2.0f, 0}  // kLeftTop
	    };

	    return center + offsetTable[static_cast<uint32_t>(corner)];
    }

} // namespace Atrum