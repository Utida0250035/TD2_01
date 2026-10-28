#include "SEPlayer.h"
#include "Audio/Audio.h"

using namespace Atrum;

std::unique_ptr<SEPlayer> SEPlayer::instance_ = nullptr;

SEPlayer* SEPlayer::GetInstance() {
	if (!instance_) {
		instance_ = std::unique_ptr<SEPlayer>(new SEPlayer);
	}
	return instance_.get();
}

void SEPlayer::Update() {
	for (auto& sound : sounds) {
		sound.second = true;
	}
}

void SEPlayer::Play(const std::string& filePath) {
	auto it = sounds.find(Audio::Manager::GetInstance()->Get(filePath));
	if (it == sounds.end()) {
		sounds.insert({Audio::Manager::GetInstance()->Get(filePath), false});
		
		Audio::Manager::GetInstance()->Play(filePath);
	} else {
		if (sounds[Audio::Manager::GetInstance()->Get(filePath)]) {
			sounds[Audio::Manager::GetInstance()->Get(filePath)] = false;
			Audio::Manager::GetInstance()->Play(filePath);
		}
	}

}
