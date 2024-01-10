#include "Pause.h"

//インスタンス化
Pause* Pause::GetInstance()
{
	static Pause pause_;
	return &pause_;
}
