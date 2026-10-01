#include "../KeVectorUtility.h"
#include "../Math/AeVector4.h"
#include "../System/DrawComponent.h"
#include <KamataEngine.h>

namespace Atrum {

class CmpMesh : public DrawComponent {
private:
	KamataEngine::WorldTransform worldTransform_{};
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::ObjectColor color_{};
	KamataEngine::Camera* camera_ = nullptr;

	Math::Vector4 colorVec_ = Math::Vector4::White();

public:
	void Initialize() override;

	void Draw() override;

	~CmpMesh() override {
		delete model_;
		model_ = nullptr;
	}

	void SetModel(KamataEngine::Model* model) { model_ = model; }
	void SetColor(const Math::Vector4& color) { colorVec_ = color; }
};

} // namespace Atrum