#include "../KeVectorUtility.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../System/DrawComponent.h"
#include <KamataEngine.h>
#include <vector>

namespace Atrum {

class CmpMeshSphere : public DrawComponent {

private:
	KamataEngine::WorldTransform worldTransform_{};
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::ObjectColor color_{};
	KamataEngine::Camera* camera_ = nullptr;

	Math::Vector4 colorVec_ = Math::Vector4::White();

	float radius_ = 1.0f;

public:
	void ResolveDependence() override;

	void Initialize() override;

	void Draw() override;

	~CmpMeshSphere() override {
		delete model_;
		model_ = nullptr;
	}

	void SetColor(const Math::Vector4& color) { colorVec_ = color; }

	void SetRadius(const float radius) { radius_ = radius; }
};

} // namespace Atrum