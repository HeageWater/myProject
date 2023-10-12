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

class FlameWork
{
public:
	virtual void Update();
	virtual void Initialize();
	virtual void Draw() = 0;
	virtual void Finalize();
	virtual void Run();
	bool& IsEndRequst() { return endRequest_; };
	void SetEndRwqust(bool flag) { endRequest_ = flag; };

	virtual~FlameWork() = default;

protected:
	//終了フラグ
	bool endRequest_ = false;

	//windowApi
	std::unique_ptr<Window> win;

	//buff
	std::unique_ptr<ConstBuff> cBuff;

	//pipeline
	std::unique_ptr<GPipeline> pipeline;

	//描画初期化
	std::unique_ptr<GPipeline> multipathPipeline;

	//gpipeline
	std::unique_ptr<GPipeline> uiPipeline;

	//shader
	Shader shader;
	Shader bilShader;
	Shader spriteShader;
};