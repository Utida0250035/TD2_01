#include "../System/CmpDrawHitSphere.h"
#include "../KeMainCamera.h"
#include "../KeMatrix3D.h"
#include "../KeVectorUtility.h"
#include "../System/CmpHitSphere.h"
#include "../System/Entity.h"
#include <KamataEngine.h>
#include <iostream>

namespace Atrum {

void CmpDrawHitSphere::ResolveDependence() {
	model_ = KamataEngine::Model::CreateFromOBJ("unitSphere");
	cmpHitSphere_ = RefOwner().GetUpdCmp<CmpHitSphere>();

	worldTransforms_.resize(cmpHitSphere_->GetSpheres().size());

	for (auto& worldTransform : worldTransforms_) {

		worldTransform = new KamataEngine::WorldTransform();
	}
}

void CmpDrawHitSphere::Initialize() {
	for (auto& worldTransform : worldTransforms_) {
		worldTransform->Initialize();
	}

	color_.Initialize();
	camera_ = KeMainCamera::GetInstance()->camera_;
}

void CmpDrawHitSphere::Draw() {

	if (!cmpHitSphere_->GetIsActive())
		return;

	color_.SetColor(ToKamataEngine(colorVec_));

	std::vector<HitSphere> hitSpheres = cmpHitSphere_->GetSpheres();

	for (size_t i = 0; i < worldTransforms_.size(); ++i) {

		worldTransforms_[i]->scale_ = {hitSpheres[i].radius, hitSpheres[i].radius, hitSpheres[i].radius};
		worldTransforms_[i]->translation_ = ToKamataEngine(GetOwner()->GetWorldPosition() + hitSpheres[i].localOffset);

		UpdateWorldTransform(*worldTransforms_[i]);
		model_->Draw(*worldTransforms_[i], *camera_, &color_);
	}
}

} // namespace Atrum