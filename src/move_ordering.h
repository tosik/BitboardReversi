/** @file
 * @brief	Move Ordering評価関数
 *
 * @author	Toshiyuki Hirooka
 *
 */

#include "bitboard_reversi.h"
using namespace Fujitalab::Reversi;


/** 開放度を取得
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @return	開放度
 */
int getOpenValue(Bitboard mobility, BitboardReversi *o, int depth);

/** 開放度理論に基づいて優秀なビットを取得
 *
 * [TODO] Potential Mobility という名前で知られている様子
 * 詳細は potential mobility reversi 等でGoogle検索
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ（2以上で発展的開放度理論）
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
Bitboard getBitByOpenTheory(Bitboard mobility, BitboardReversi *o, int depth);

/** ネガアルファにより評価値を取得する
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @param	alpha	αカット用変数
 * @param	beta	βカット用変数
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
int getValueByNegaalpha(Bitboard mobility, BitboardReversi *o, int depth, int alpha, int beta);

/** ネガアルファを用いて最も優秀な手を取得する
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
Bitboard getBitByNegaalpha(Bitboard mobility, BitboardReversi *o, int depth);

/** 期待値法に基づいて期待値を取得する
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @param	alpha	αカット用変数
 * @param	beta	βカット用変数
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
float getExpectation(Bitboard mobility, BitboardReversi *o, int depth, float alpha, float beta);

/** 期待値法に基づいて最も優秀な手を取得する
 * 期待値法は http://ci.nii.ac.jp/naid/110002771150/ を用いている
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
Bitboard getBitByExpectationMethod(Bitboard mobility, BitboardReversi *o, int depth);

/** N手先の着手可能な手数が最小のものを取得
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
int getMNM(Bitboard mobility, BitboardReversi *o, int depth);

/** N手先の着手可能な手数が最小になる手を取得
 * 部分木が小さい手やパスさせる手が優先的に選択される
 *
 * MNM法 (Minimum Number of Mobilities Method)
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
Bitboard getBitByMNM(Bitboard mobility, BitboardReversi *o, int depth);

/** ランダムで手を決定
 *
 * @param	mobility	着手可能場所のビットボード
 * @return	ランダムで選択された着手可能な場所のビットだけが1のビットボード
 */
Bitboard getBitByRandom(Bitboard mobility);

/** ビット列のテーブルを参照し、その中で最も優秀なビットだけを取得
 *
 * @param bits	ビット列
 * @return		優秀なビットだけが1のビットボード
 */
Bitboard getBitByTable(Bitboard bits);

