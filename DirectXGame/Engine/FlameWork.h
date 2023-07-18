#pragma once

class FlameWork
{
public:
	virtual void Update();
	virtual void Initilize();
	virtual void Draw() = 0;
	virtual void Finalize();
	void Run();
	bool IsEndRequst() { return endRequest_; };
	void SetEndRwqust(bool flag) { endRequest_ = flag; };

	virtual~FlameWork() = default;
private:

	//終了フラグ
	bool endRequest_ = false;

	//windowApi
	//std::unique_ptr<Window> win;

	////dxCommon
	//std::unique_ptr<MyDirectX> dx;

	////buff
	//std::unique_ptr<ConstBuff> cBuff;

	////pipeline
	//std::unique_ptr<GPipeline> pipeline;

	////描画初期化
	//std::unique_ptr<GPipeline> multipathPipeline;

	////gpipeline
	//std::unique_ptr<GPipeline> uiPipeline;

};