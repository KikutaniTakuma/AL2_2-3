#pragma once

class Frame {
public:
	Frame(void);

public:
	// フレーム
	unsigned long long frame;

public:
	// スタートフラグ
	bool startFlag;

	// スタート関数
	void Start(void);

	// 停止
	void Pause(void);

	// ストップ
	void Stop(void);

	// リスタート
	void Restart(void);

	int operator%(int num);
};