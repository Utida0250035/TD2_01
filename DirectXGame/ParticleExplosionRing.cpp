#include "ParticleExplosionRing.h"

#include "System/CmpMesh.h"
#include "Time/DeltaTime.h"

using namespace Atrum;

void ParticleExplosionRing::Initialize() {

	// エンティティ生成
	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::NONE);
	entity_->SetInitialTransform({{}, {}, initialTranslation_});

	// コンポーネント追加
	entity_->AddDrawCmp<CmpMesh>();
	CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("explosionRing", false));

	ParticleInfo info{};
	info.lifeTimer_ = 1.0f;
	particleInfo_.push_back(info);
}

void ParticleExplosionRing::Update() {

	for (auto& info : particleInfo_) {

		info.lifeTimer_ -= 1.0f / 60.0f * FrameDeltaTime::GetInstance()->GetDeltaTime();

		if (info.lifeTimer_ <= 0.0f) {
			isFinish_ = true;
		}
	}
}

void ParticleExplosionRing::Draw() { entity_->Draw(); }

void ParticleExplosionRing::Finalize() { entity_.release(); }

ParticleExplosionRing::~ParticleExplosionRing() { Finalize(); }