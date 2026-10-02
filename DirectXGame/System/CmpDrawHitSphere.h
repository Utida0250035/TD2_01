#include "../KeVectorUtility.h"
#include "../Math/Vector4.h"
#include "../System/DrawComponent.h"
#include <KamataEngine.h>
#include <vector>
#include <memory>

namespace Atrum {

class CmpHitSphere;

class CmpDrawHitSphere : public DrawComponent {
private:
	std::vector<KamataEngine::WorldTransform*> worldTransforms_{};
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::ObjectColor color_{};
	KamataEngine::Camera* camera_ = nullptr;
	CmpHitSphere* cmpHitSphere_ = nullptr;

	Math::Vector4 colorVec_ = Math::Vector4::White();

public:
	void ResolveDependence() override;

	void Initialize() override;

	void Draw() override;

	~CmpDrawHitSphere() override {
		delete model_;
		model_ = nullptr;

		for (auto& worldTransform : worldTransforms_) {
		
			delete worldTransform;
			worldTransform = nullptr;
		
		}

		worldTransforms_.clear();

	}

	void SetColor(const Math::Vector4& color) { colorVec_ = color; }
};

} // namespace Atrum