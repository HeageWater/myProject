#pragma once

class Actor {

protected:
	static void Initialize();
	//static void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	static void Draw();
	//static void Draw(size_t tex);
	static void Update();
	//static void Update(Matrix matView, Matrix matProjection);

	Model actor;
};
