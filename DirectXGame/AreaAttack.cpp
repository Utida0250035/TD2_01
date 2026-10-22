#include "AreaAttack.h"

#include "../KeMainCamera.h"
#include "../KeMatrix3D.h"
#include "Time/DeltaTime.h"

#include "SEPlayer.h"

using namespace Atrum;
using namespace Atrum::Math;

void AreaAttack::Initialize(const Math::Vector3& emitPos) {

	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::ENEMY);
	entity_->SetInitialTransform({
	    scale_,
        {},
        emitPos
    });

	entity_->AddDrawCmp<CmpMesh>();
	cmpMesh_ = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh_->SetModel(KamataEngine::Model::CreateFromOBJ("blockRounded", true));
	cmpMesh_->SetColor({1.0f, 1.0f, 0.1f, 0.3f});

	warnSprite_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("warning.png"), {}, {1, 1, 1, 1}, {0.5f, 0.5f});

	entity_->Update();

	entity_->ResolveDependences();
	entity_->Initialize();

}

void AreaAttack::Update() {

	timer_ += FrameDeltaTime::GetInstance()->GetDeltaTime();
	intTimer_++;

	if (timer_ < attackStartTime_) {
	
		Matrix4x4 matViewport = Matrix4x4::Viewport(0.0f, 0.0f, float(KamataEngine::WinApp::kWindowWidth), float(KamataEngine::WinApp::kWindowHeight), 0.0f, 1.0f);
		Matrix4x4 matView = FromKamataEngine(KeMainCamera::GetInstance()->camera_->matView);
		Matrix4x4 matProjection = FromKamataEngine(KeMainCamera::GetInstance()->camera_->matProjection);
		Matrix4x4 matViewProjection = matView * matProjection;

		Vector3 screenPos = Matrix4x4::ScreenTransform(entity_->GetTransform().translate, matViewProjection, matViewport);
		warnSprite_->SetPosition({screenPos.x, screenPos.y});

	}

	if (attackEntity_ == nullptr) {
		if (timer_ >= attackStartTime_) {
		
			attackEntity_ = std::make_unique<Entity>();
			attackEntity_->SetEntityType(EntityType::ENEMY_WEAPON);
			attackEntity_->SetInitialTransform({
			    {scale_.x / 2.0f, scale_.x / 2.0f, scale_.x / 2.0f},
                {},
                {entity_->RefTransform().translate.x, entity_->RefTransform().translate.y + (entity_->RefTransform().scale.y / 2.0f), 0.0f}
            });
			
			attackEntity_->AddUpdCmp<CmpHitSphere>();
			CmpHitSphere* cmpHitSphere = attackEntity_->GetUpdCmp<CmpHitSphere>();
			cmpHitSphere->SetSpheres({
			    {{}, scale_.x / 2.0f}
            });

			attackEntity_->AddDrawCmp<CmpMesh>();
			CmpMesh* cmpMesh = attackEntity_->GetDrawCmp<CmpMesh>();
			cmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("fallCloud", true));
			cmpMesh->SetColor({1.0f, 1.0f, 0.3f, 1.0f});

			attackEntity_->ResolveDependences();
			attackEntity_->Initialize();

			// もう使わないものはあらかじめ消しておく
			entity_->SetState(Entity::State::Sleep);
			entity_->Finalize();
			delete warnSprite_;

			SEPlayer::GetInstance()->Play("./Resources/seAreaAttack.mp3");

		}
	}

	if (attackEntity_) {
		attackEntity_->RefTransform().translate.y -= 50.0f * FrameDeltaTime::GetInstance()->GetDeltaTime();
		attackEntity_->Update();

		// 消去
		if (attackEntity_->RefTransform().translate.y <= -40.0f) {
		
			//entity_->Finalize();
			attackEntity_->Finalize();
			isActive_ = false;

		}

	}

}

void AreaAttack::Draw() {

	if (timer_ < attackStartTime_) {
	
		if (intTimer_ % 2 == 0) {

			KamataEngine::Model::PreDraw(KamataEngine::ModelCommon::CullingMode::kBack, KamataEngine::Model::BlendMode::kNormal, KamataEngine::Model::DepthTestMode::kReadOnly);

			entity_->Draw();

			KamataEngine::Model::PostDraw();
		}

		if (warnSprite_) {

			KamataEngine::Sprite::PreDraw();

			warnSprite_->Draw();

			KamataEngine::Sprite::PostDraw();
		}

	}

	if (attackEntity_) {

		KamataEngine::Model::PreDraw();

		attackEntity_->Draw();

		KamataEngine::Model::PostDraw();

	}

}