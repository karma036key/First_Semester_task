
// ==============================
// 2020年 前期 
// ゲームプログラミング１
// 制作課題 その２
// ==============================

/**
■説明
	以下のプログラムは [ 〇×(三目並べ) ] というゲームをコーディングし、
	実行を確認したうえでその一部分を削除したものです。

	削除した部分は　※※　と記載し、
	本来入るべき処理の内容をコメントしてあります。

	コメント内容や他のコードを読み解き、ゲームを完成させなさい。

	※Hit And Blowに比べてコメントのヒントが少ないので
	　どのような処理を書けばよいか良く考えましょう
 */

/**
■制作環境
	このプログラムは、[ ＤＸライブラリ ] というライブラリを用いて作られています。
		ホームページ：https://dxlib.xsrv.jp/
		リファレンス：https://dxlib.xsrv.jp/dxfunc.html

	ＤＸライブラリについて興味のある人は、
	ホームページ、リファレンスページ（用意されている関数の説明）を見てみてください。
*/

/**
■〇×(三目並べ)のルール
	ルールについては、自分で調べて把握するようにしてください
*/

#include "WinMain.h"
#include "InputManager.h"
#include "DrawManager.h"
#include <stdio.h>
#include <time.h>

// ==============================
// グローバル変数
// ==============================
// 横:STAGE_WIDTH、縦:STAGE_HEIGHTのint型２次元配列 map を宣言
int map[STAGE_WIDTH][STAGE_HEIGHT];


// ==============================
// 関数プロトタイプ宣言
// ==============================
// 盤面の x, y の位置に石が置けるかどうか
 // bool型の戻り値、int型の引数x,yを持つIsPutStone関数を宣言
bool IsPutStone(int x, int y);
// 勝者が居るかを調べる
// int型の戻り値を持つCheckWinner関数を宣言
int CheckWinner(int turn);

// ==============================
// Main関数
// ==============================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ＤＸライブラリを使う上での初期化処理
	// ----------------------------------------------------
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetGraphMode(WINDOW_W, WINDOW_H, 32);
	SetBackgroundColor(125, 125, 255);
	SetMainWindowText("マルバツ");
	if (DxLib_Init() == -1) { return -1; }

	SetDrawScreen(DX_SCREEN_BACK);

	// ローカル変数宣言
	// ----------------------------------------------------
	//整数型の変数pos_xを宣言し、0で初期化		// X座標選択位置
	//整数型の変数pos_yを宣言し、0で初期化		// Y座標選択位置
	//整数型の変数turnを宣言し、STONE_WHITEで初期化	// 現在の手番
	//整数型の変数winnerを宣言し、WINNER_NONで初期化	// 勝利者
	int pos_x = 0;
	int	pos_y = 0;
	int turn = STONE_WHITE;
	int winner = WINNER_NON;

	// 各種初期化処理
	// ----------------------------------------------------
	InputInit();			// 入力処理初期化関数の呼び出し
	DrawInit();				// 描画処理初期化関数の呼び出し

	// mapの初期化
	//二次元配列mapの全要素を STONE_MAX で初期化する
	for (int i = 0; i < STAGE_WIDTH; i++)
	{
		for (int j = 0; j < STAGE_HEIGHT; j++)
		{
			map[i][j] = STONE_MAX;
		}
	}
	// ゲームのメインループ
	// 画面を１回表示する毎にwhile分を１回処理する
	// ----------------------------------------------------
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ＤＸライブラリを使う上で、１フレーム分の処理を始めるためのお約束
		// ----------------------------------------------------
		ClearDrawScreen();
		clsDx();

		// 以下、毎フレーム更新する処理
		// ----------------------------------------------------
		InputUpdate();			// 入力処理更新関数の呼(び出し
		CheckWinner(turn);//勝利者情報入力	// 勝利者のチェック

		// --- 入力状況をチェックして、適切な処理を行う
		// 決着がついてない時だけ入力を受け付けるように if文 でチェックする
		if (winner == WINNER_NON)
		{
			// 上下左右の入力があった時の処理
			if (IsPushKey(MY_INPUT_DOWN))
			{
				pos_y -= 1;
			}
			else if (MY_INPUT_UP)
			{
				pos_y += 1;
			}
			else if (MY_INPUT_RIGHT)
			{
				pos_x -= 1;
			}
			else if (MY_INPUT_LEFT)
			{
				pos_x += 1;
			}
			// 決定(=エンターキー)が押された時の処理
			else if (MY_INPUT_ENTER)
			{
				// 現在の座標が有効か if文 でチェックし、
				// 結果が true の時、以下の処理を行う
				if (IsPutStone(pos_x, pos_y))
				{

					// 以下の処理を実装する
					// 選択されている座標と対応するmap配列の要素へturnの値を代入
					// 次のターンに回すため、turnの値を変更する
					map[pos_x][pos_y] = turn % 2;
					turn++;
				}
			}


			// 以下、描画処理
			// ----------------------------------------------------
			DrawInformation(turn);	// 情報文章を描画
			DrawGameClear(winner);	// ゲームクリアの文字を描画
			DrawBgLine();	// 枠線を描画
			DrawStone(pos_x, pos_y, turn % 2); //２重for文を使って盤面の石を描画する
			DrawCursor(pos_x, pos_y); //カーソルを描画

			// ＤＸライブラリを使う上で、モニターへゲーム画面を表示するためのお約束
			// 必ずループの最後で呼び出す
			// ----------------------------------------------------
			ScreenFlip();
		}

		// 後始末
		// ----------------------------------------------------
		DrawEnd();		// 描画処理終了

		// ＤＸライブラリを使う上での終了処理
		// ----------------------------------------------------
		DxLib_End();
		return 0;
	}
}
// ==============================
// 盤面の x, y の位置に石が置けるかどうか
// true = 置ける
// false = 置けない
// ==============================
bool IsPutStone(int x, int y)
{
	if (map[y][x] == STONE_MAX)	//　盤面の x, y の位置に石が置けるならtrue, 置けないならfalseを返す処理
	{
		return true;
	}
	else
	{
		return false;
	}
}



// ==============================
// 勝者が居るかを調べる
// ==============================
int CheckWinner(int turn)
{
	//※※　以下の処理を実装する
	// 縦、横、斜めが同じ石かどうかを調べる
	// STONE_WHITE, STONE_BLACK, STONE_MAXを上手く使いましょう

	// もし、まだ揃っていなかったら、盤面に置かれている石の数を調べる
	// 全てのマスに石が置かれていたら引き分け

	// 上記のいずれかでも無かったらWINNER_NONを返す
	int count[9];
	
	for (int i = 0; i < 9; i++)
	{		count[i] = {};	}
		
	for (int x = 0; x < STAGE_HEIGHT; x++)
	{
		for (int y = 0; y < STAGE_WIDTH; y++)
		{
			if (map[y][y] == turn % 2)
			{
				count[0];
			}
			if (map[y][x] == turn % 2)
			{
				if (y == 1)
				{
					count[1]++;
				}
				else if (y == 2)
				{
					count[2]++;
				}
				else if (y == 3)
				{
					count[3]++;
				}

				if (x == 1)
				{
					count[4]++;
				}
				else if (x == 2)
				{
					count[5]++;
				}
				else if (x == 3)
				{
					count[6]++;
				}
			}
			if (map[2][0] == turn % 2|| map[1][1] == turn % 2|| map[0][2] == turn % 2)
			{
				count[7]++;
			}
			if (map[y][x] == turn % 2)
			{
				count[8]++;
			}
		}
	}
	for (int i = 0; i < 9; i++)
	{
		if (count[8]==STAGE_WIDTH*STAGE_HEIGHT)
		{
			return WINNER_DRAW;
		}
		else if (count[i] == 3)
		{
			return turn % 2;
		}
		else
		{
			return WINNER_NON;
		}
	}
}
