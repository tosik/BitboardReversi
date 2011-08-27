/** @file
 * @brief	Bitboard Reversi ヘッダファイル
 * @author	Toshiyuki Hirooka
 *
 * Bitboard を用いた NxM に対応のリバーシ
 *
 */


#pragma once


#include <ctime>
#include <iostream>
#include <map>


#include "setting.h"

#include "bitboard_reversi_functions.h"


namespace Fujitalab
{
	namespace Reversi
	{

		/** Bitboard版のリバーシクラス
		 *
		 */
		class BitboardReversi
		{
		public:

			/** 板の状態(黒石) */
			Bitboard black;
			/** 板の状態(白石) */
			Bitboard white;

			/** 手番(true = 黒、 false = 白) */
			bool current_color;

			/** stack(undo用) */
			BoardStack board_stack;

			/** 交代したかを監視する変数 */
			bool changed;

			/** コンストラクタ(オブジェクトの初期化) */
			BitboardReversi();

			/** 設置する（ひっくり返さない）
			 *
			 * @param b	設置する座標のみが1のビットボード
			 * @param color	設置する色
			 */
			void set(Bitboard b, bool color);

			/** 着手する（ひっくり返す）
			 *
			 * 高速化用に最適化されているため、特定の条件でのみ正しく動く
			 * @param m	着手する座標のビットボード
			 */
			void put(Bitboard m);

			/** x,y を使って着手する（ひっくり返す）
			 *
			 * @param x	着手するX座標
			 * @param y	着手するY座標
			 */
			void put(int x, int y);

			/** 着手する（ひっくり返す）安全版
			 *
			 * putが高速化用に最適化されているのに対して、
			 * こちらは低速だが状態に依存することなくいつでも正しく動くことを保障している（つまり安全）
			 * @param m	着手する座標のビットボード
			 */
			void putSafety(Bitboard m);

			/** x,y を使って着手する安全版
			 *
			 * @param x	着手するX座標
			 * @param y	着手するY座標
			 */
			void putSafety(int x, int y);

			/** 1手戻す
			 *
			 */
			void undo();

			/** 現在の局面の状態を Board 型にする
			 *
			 * @return	局面の状態
			 */
			Board toBoard();

			/** 局面の状態を与える
			 *
			 * @param black	黒の状態
			 * @param white	白の状態
			 * @param current_color	手番
			 */
			void setBoard(Bitboard black, Bitboard white, bool current_color);

			/** 現在の局面を表示
			 *
			 */
			void view();

			/** 現在の局面の着手可能箇所を表示
			 *
			 */
			void viewMobility();

			/** 着手可能な箇所をビットボードで取得
			 *
			 * @return	着手可能な箇所のビットボード
			 */
			Bitboard getMobility();

			/** 与えた局面において、着手可能な箇所をビットボードで取得
			 *
			 * @param black	黒の状態
			 * @param white	白の状態
			 * @return	着手可能な箇所のビットボード
			 */
			Bitboard getMobility(Bitboard black, Bitboard white);

			/** 着手可能を検査するための特殊な変数
			 *
			 */
			Bitboard r_Left;
			Bitboard r_Right;
			Bitboard r_Top;
			Bitboard r_Bottom;
			Bitboard r_LeftTop;
			Bitboard r_LeftBottom;
			Bitboard r_RightTop;
			Bitboard r_RightBottom;

			/** 着手後に反転する石が示されているビットボードを取得
			 *
			 * @param mob	着手するビットボード
			 * @return	反転する石のパターンのビットボード
			 */
			Bitboard getReversedPattern(Bitboard mob);

		};
	}
}
