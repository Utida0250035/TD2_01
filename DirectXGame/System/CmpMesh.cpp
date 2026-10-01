#include "../System/CmpMesh.h"
#include "../KeMainCamera.h"
#include "../KeMatrix3D.h"
#include "../System/Entity.h"

namespace Atrum {

void CmpMesh::Initialize() {
	worldTransform_.Initialize();
	color_.Initialize();
	camera_ = KeMainCamera::GetInstance()->camera_;
}

void CmpMesh::Draw() {

	UpdateWorldTransform(worldTransform_, RefOwner().GetWorldMatrix());

	color_.SetColor(ToKamataEngine(colorVec_));

	model_->Draw(worldTransform_, *camera_, &color_);
}

} // namespace Atrum