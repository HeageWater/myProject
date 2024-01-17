#pragma once
#include "Window.h"
#include "DirectX.h"
#include "JsonFileOpen.h"
#include "ConstBuff.h"
#include "GPipeline.h"
#include "Shader.h"
#include "Square.h"
#include <memory>
#include <random>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <map>
#include "imgui.h"
#include <wrl.h>
#include "ImguiManager.h"
#include "Controller.h"

/// <summary>
/// フレームワーク
/// </summary>
class FlameWork
{
public:

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 描画(無し)
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 破棄
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// ゲームループ
	/// </summary>
	virtual void Run();

	/// <summary>
	/// 終了条件が達成しているか
	/// </summary>
	/// <returns></returns>
	bool& IsEndRequst() { return endRequest_; };

	/// <summary>
	/// 終了フラグをセット
	/// </summary>
	/// <param name="flag"></param>
	void SetEndRwqust(bool flag) { endRequest_ = flag; };

	virtual~FlameWork() = default;

protected:
	//終了フラグ
	bool endRequest_ = false;

	//windowApi
	std::unique_ptr<Window> win_;

	//buff
	std::unique_ptr<ConstBuff> cBuff_;

	////pipeline
	//std::unique_ptr<GPipeline> pipeline_;

	////描画初期化
	//std::unique_ptr<GPipeline> multipathPipeline_;

	////gpipeline
	//std::unique_ptr<GPipeline> uiPipeline_;

	////shader
	//Shader shader_;
	//Shader bilShader_;
	//Shader spriteShader_;

	//BGM
	size_t hitSound_;
};