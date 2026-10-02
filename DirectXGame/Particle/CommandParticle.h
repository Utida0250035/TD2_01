#pragma once

#include "../Hash/Hash64.h"
#include "Particle.h"
#include <cassert>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

namespace Atrum {

class CommandParticle final {

private:
	CommandParticle();
	~CommandParticle() = default;

	struct ParticleContain {
		size_t handle;
		std::unique_ptr<Particles> particle;
	};

	std::vector<ParticleContain> particleContainer_;
	std::unordered_map<uint64_t, size_t> handleTable_;

	size_t GetHandle(const std::string& particleName) {

		// パーティクル名でハンドルを検索
		auto search = handleTable_.find(Hash64(particleName));

		if (search != handleTable_.end()) {
			// コンテナ内に検索対象のハンドルが存在すれば

			// 検索対象のインスタンスハンドルを参照元へ渡す
			return search->second;
		}

		// テーブルにインスタンスハンドルを追加
		handleTable_.emplace(Hash64(particleName), handleTable_.size());

		// 追加したハンドルを参照元へ渡す
		return handleTable_.size() - 1;
	}

public:
	CommandParticle(const CommandParticle& source) = delete;
	CommandParticle& operator=(const CommandParticle& source) = delete;

	static CommandParticle* GetInstance() {

		static CommandParticle instance;

		return &instance;
	}

	void Generate(const std::string& particleName, Particles* particle) {

		// 末尾要素として構築
		particleContainer_.emplace_back(ParticleContain{GetHandle(particleName), nullptr});
		particleContainer_.rbegin()->particle.reset(particle);
	}

	/// <summary>
	/// パーティクル更新処理
	/// </summary>
	void Update() {

		// 完了フラグが立っている要素と空ポインタを全て削除
		std::erase_if(particleContainer_, [&](const ParticleContain& contain) {
			if (!contain.particle) {

				return true;
			}

			return contain.particle->GetIsFinish();
		});

		for (auto& particleContainer : particleContainer_) {

			// パーティクルの更新
			particleContainer.particle->Update();
		}
	}

	void DestroyParticle(const std::string& particleName) {

		// 合致する名前の要素と空ポインタを削除
		std::erase_if(particleContainer_, [&](const ParticleContain& contain) {
			if (!contain.particle) {

				return true;
			}

			return contain.handle == GetHandle(particleName);
		});
	}

	void DestroyParticleAll() { particleContainer_.clear(); }

	bool IsExist(const std::string& particleName) {

		auto search = handleTable_.find(Hash64(particleName));

		if (search == handleTable_.end())
			return false;

		for (auto& particleBox : particleContainer_) {
			if (particleBox.handle == search->second)
				return true;
		}

		return false;
	}

	/// <summary>
	/// パーティクル描画処理
	/// </summary>
	/// <param name="particleName"> パーティクルに付けた名前 </param>
	void Draw(const std::string& particleName) {

		for (const auto& particleContainer : particleContainer_) {

			if (!particleContainer.particle) {

				continue;
			}

			if (particleContainer.handle == GetHandle(particleName)) {

				particleContainer.particle->Draw();
			}
		}
	}
};

} // namespace Atrum