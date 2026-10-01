#include "../System/CmpMeshSphere.h"
#include "../KeMainCamera.h"
#include "../KeMatrix3D.h"
#include "../KeVectorUtility.h"
#include "../System/Entity.h"

namespace Atrum {

void CmpMeshSphere::ResolveDependence() { model_ = KamataEngine::Model::CreateFromOBJ("unitSphere"); }

void CmpMeshSphere::Initialize() {

	worldTransform_.Initialize();
	color_.Initialize();
	camera_ = KeMainCamera::GetInstance()->camera_;
}

void CmpMeshSphere::Draw() {

	worldTransform_.scale_ = ToKamataEngine(radius_ * RefOwner().GetTransform().scale);
	worldTransform_.translation_ = ToKamataEngine(GetOwner()->GetWorldPosition());

	UpdateWorldTransform(worldTransform_);

	color_.SetColor(ToKamataEngine(colorVec_));

	model_->Draw(worldTransform_, *camera_, &color_);
}

} // namespace Atrum