#include "./ForDebug/DebugConsole.h"
#include "./Scene/CommandChangeScene.h"
#include "./Scene/GameOrder.h"
#include "./Scene/SceneTitle.h"
#include "./System/EntityFactory.h"
#include "./System/EntityStorage.h"
#include "./System/Playable/CmpInput.h"
#include "./Time/DeltaTime.h"
#include "Audio/Audio.h"
#include "KeMainCamera.h"
#include <KamataEngine.h>
#include <Windows.h>

/*

チーム: チーム番号_ゲームタイトル

個人: クラス記号_出席番号_氏_名_タイトル

*/

const wchar_t* kWindowTitle = L"LE2A_02_ウチダ_コウタ_跳斬";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	///
	/// ↓初期化処理ここから
	///

	Atrum::Debug::OpenDebugConsole();

	// エンジンの初期化
	KamataEngine::Initialize(kWindowTitle);

	{

		HWND hWnd = KamataEngine::WinApp::GetInstance()->GetHwnd();

		// 現在のスタイルを取得
		LONG style = GetWindowLong(hWnd, GWL_STYLE);

		// WS_THICKFRAME（サイズ変更枠）を削除
		style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

		// スタイルを再設定
		SetWindowLong(hWnd, GWL_STYLE, style);

		// 変更を反映させるためフレームを再描画
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	// ImGuiマネージャーの初期化
	KamataEngine::ImGuiManager::GetInstance()->Initialize();

	// メインカメラの初期化
	KeMainCamera::GetInstance()->Initialize();

	// 実経過時間インスタンスの初期化
	Atrum::FrameDeltaTime::GetInstance()->Initialize();

	// 抽象入力コンポーネントの依存関係解決
	Atrum::Player::CmpInput::GetInstance()->ResolveDependence();

	Atrum::Audio::Manager* audio = Atrum::Audio::Manager::GetInstance();

	// 音源マネージャーの初期化
	audio->Initialize();

	// 音源の読み込み
	audio->Load("./Resources/bgmYasha.mp3");
	audio->Load("./Resources/seParry.mp3");
	audio->Load("./Resources/seHit.mp3");
	audio->Load("./Resources/seStep.mp3");
	audio->Load("./Resources/seSwingWeapon.mp3");
	audio->Load("./Resources/seEnemyShot.mp3");

	// エンティティの作成

	Atrum::EntityStorage::GetInstance()->Register("playerMononofu", Atrum::EntityFactory::PlayerMononofu());

	Atrum::EntityStorage::GetInstance()->Register("enemyAstro", Atrum::EntityFactory::EnemyAstro(Atrum::EntityStorage::GetInstance()->Find("playerMononofu").ptr_));

	// ゲーム実行インスタンスの取得
	Atrum::GameOrder* order = Atrum::GameOrder::GetInstance();

	// シーンの初期化
	Atrum::CommandChangeScene::GetInstance()->Set(Atrum::SceneTitle::GetInstance());

	///
	/// ↑初期化処理ここまで
	///

	while (true) {

		// エンジンの更新
		if (KamataEngine::Update()) {

			break;
		}

		order->Run();

		audio->Update();
	}

	///
	/// ↓終了処理ここから
	///

	KamataEngine::Finalize();

	return 0;

	///
	/// ↑終了処理ここまで
	///
}