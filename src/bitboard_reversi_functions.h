/** @file
 * @brief	Bitboard Reversi 関数のヘッダファイル
 *
 * @author	Toshiyuki Hirooka
 */

#pragma once


#include <stack>

#include "setting.h"


/** 藤田研究室 */
namespace Fujitalab
{
	/** リバーシ */
	namespace Reversi
	{

		// ボードの最大サイズ（Bitboard は最大 64bits の変数を用いるため、これ以上は不可能。将来的な拡張のための定数）
		/** ボードのX軸最大サイズ */
		const int MAX_SIZE_X = 8;
		/** ボードのY軸最大サイズ */
		const int MAX_SIZE_Y = 8;
		/** ボードの最大サイズ */
		const int MAX_SIZE = (MAX_SIZE_X*MAX_SIZE_Y);

		/** Bitboard型の定義
         *
         * 各ビットは局面の1マスに対応している。
         * 64bitを縦8横8のビット列と考える\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         *
         * これは様々な場面で使われている。たとえば局面を表現するには黒のビットボードと白のビットボードの二つを用いる。
         * 着手可能な座標を示す場合もビットボードで表現できる。詳しくは各定義箇所を参照のこと。
         */
		typedef unsigned long long int Bitboard;

		/** 簡易版Board型の定義（局面の状態）*/
		typedef struct
		{
			/** 黒石のビットボード */
			Bitboard black;
			/** 白石のビットボード */
			Bitboard white;
			/** 手番（先手ならtrue, 後手ならfalse） */
			bool current_color;
		} SimpleBoard;

		/** Board型の定義（局面の状態）*/
		typedef struct
		{
			/** 黒石のビットボード */
			Bitboard black;
			/** 白石のビットボード */
			Bitboard white;
			/** 手番（先手ならtrue, 後手ならfalse） */
			bool current_color;
			/** 演算処理を高速に行うために使うフラグ（難しいことしてます） */
			Bitboard left, right, top, bottom, lefttop, leftbottom, righttop, rightbottom;
		} Board;

		/** UNDOなどに用いるボードの状態を保存するスタック */
		typedef std::stack<Board> BoardStack; // [TODO] 標準ライブラリを使わず、自力で実装して高速化する

		/** 底２の対数を取得する
		 *
		 * @param b	対数に変換したい数値
		 * @return	対数に変換された値*/
		int log2(Bitboard b);

		/** 座標を Bitboard にする
		 *
		 * 与えた座標のビットが1,その他が0のビットボードを取得
		 * @param x	X座標の値
		 * @param y	Y座標の値
		 * @return	X,Yを元に変換したビットボード
		 */
		Bitboard toBitboard(int x, int y);

		/** ビット列のうちはじめに出現するビットだけを取得
		 *
         * 例：01101000 -> 00001000
		 * @param bits	ビット列
		 * @return		はじめに出現するビットだけが1のビットボード
		 */
		Bitboard getFirstBit(Bitboard bits);

		/** ビット列の 1 の数を取得
		 *
		 * @param n	1の数をカウントしたいビット列
		 * @return	カウントした1の数
		 */
		int count(Bitboard n);

		// 
		/** ビットボードを表示する
		 *
		 * @param b	表示したいビットボード
		 */
		void view(Bitboard b);

		/** 着手可能な手を取得(左方向にひっくり返せる)
		 *
		 * @param black	局面の黒の状態
		 * @param white	局面の白の状態
		 * @return		着手可能な座標が1のビットボード
		 */
		Bitboard getMobilityLeft(Bitboard black, Bitboard white);
		/** 着手可能な手を取得(右方向にひっくり返せる)
		 *
		 * @param black	局面の黒の状態
		 * @param white	局面の白の状態
		 * @return		着手可能な座標が1のビットボード
		 */
		Bitboard getMobilityRight(Bitboard black, Bitboard white);
		/** 着手可能な手を取得(上方向にひっくり返せる)
		 *
		 * @param black	局面の黒の状態
		 * @param white	局面の白の状態
		 * @return		着手可能な座標が1のビットボード
		 */
		Bitboard getMobilityTop(Bitboard black, Bitboard white);
		/** 着手可能な手を取得(下方向にひっくり返せる)
		 *
		 * @param black	局面の黒の状態
		 * @param white	局面の白の状態
		 * @return		着手可能な座標が1のビットボード
		 */
		Bitboard getMobilityBottom(Bitboard black, Bitboard white);
		/** 着手可能な手を取得(左上方向にひっくり返せる)
		 *
		 * @param black	局面の黒の状態
		 * @param white	局面の白の状態
		 * @return		着手可能な座標が1のビットボード
		 */
		Bitboard getMobilityLeftTop(Bitboard black, Bitboard white);
		/** 着手可能な手を取得(左下方向にひっくり返せる)
		 *
		 * @param black	局面の黒の状態
		 * @param white	局面の白の状態
		 * @return		着手可能な座標が1のビットボード
		 */
		Bitboard getMobilityLeftBottom(Bitboard black, Bitboard white);
		/** 着手可能な手を取得(右上方向にひっくり返せる)
		 *
		 * @param black	局面の黒の状態
		 * @param white	局面の白の状態
		 * @return		着手可能な座標が1のビットボード
		 */
		Bitboard getMobilityRightTop(Bitboard black, Bitboard white);
		/** 着手可能な手を取得(右下方向にひっくり返せる)
		 *
		 * @param black	局面の黒の状態
		 * @param white	局面の白の状態
		 * @return		着手可能な座標が1のビットボード
		 */
		Bitboard getMobilityRightBottom(Bitboard black, Bitboard white);

		/** ビットボードを90度に回転
		 *
		 * @param board	回転させたいビットボード
		 * @return 		回転されたビットボード
		 */
		Bitboard rotate(Bitboard board);

		/** ビットボードをある方向へ移動する（マスキング処理を兼ねる）*/
		Bitboard transferLeft(Bitboard b);
		Bitboard transferRight(Bitboard b);
		Bitboard transferTop(Bitboard b);
		Bitboard transferBottom(Bitboard b);
		Bitboard transferLeftTop(Bitboard b);
		Bitboard transferLeftBottom(Bitboard b);
		Bitboard transferRightTop(Bitboard b);
		Bitboard transferRightBottom(Bitboard b);

		// MMX が使えるか
		bool isMMXEnable();

	}
}
