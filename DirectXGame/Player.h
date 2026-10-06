#pragma once

#include "KamataEngine.h"

#include "Math/Vector3.h"
#include "Math/Transform.h"
#include "Math/Matrix4x4.h"
#include "System//CmpMesh.h"

#include "System/Entity.h"
#include "System/CmpLhRigidBody.h"

#include <algorithm>
#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <vector>

#include "../Hash/Hash64.h"
#include "../Math/Transform.h"
#include "../System/DataComponent.h"
#include "../System/DrawComponent.h"
#include "../System/EntityType.h"
#include "../System/UpdComponent.h"

// プレイヤー
class Player final {

	private:

		static inline const float gravity_ = 0.05f;

		// モデル
		KamataEngine::Model* model_ = nullptr;

		// ワールド変換データ
		//KamataEngine::WorldTransform worldTransform_;

		// ワールド変換データ
		Atrum::Math::Transform transform_;

		// カメラ
		KamataEngine::Camera* camera_ = nullptr;

		// 移動方向
		KamataEngine::Vector2 moveDirection = {};

		// ベクトル
		Atrum::Math::Vector3 velocity_ = {};

		// 入力
		KamataEngine::Input* input_ = nullptr;

		// エンティティ
	    std::unique_ptr<Atrum::Entity> entity_ = nullptr;

	public:

		// 初期化
	    void Initialize();

		// 更新
		void Update();

		// 描画
		void Draw();

};
