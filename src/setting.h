/** @file
 * @brief	Bitboard Reversi 設定ファイル
 *
 * @author	Toshiyuki Hirooka
 */

#pragma once


/** ボードのサイズX */
#ifndef SIZE_X
//#			define	SIZE_X	4
//#			define	SIZE_X	5
#endif
/** ボードのサイズY */
#ifndef SIZE_Y
//#			define	SIZE_Y	4
//#			define	SIZE_Y	5
#endif


/** アルゴリズムの種類 */
#ifndef NEGAMAX
#ifndef NEGAALPHA
#ifndef BETTER_NEGAALPHA
#ifndef OPEN_THEORY_NEGAALPHA
#ifndef NEGASCOUT
//#			define NEGAMAX
//#			define NEGAALPHA
//#			define BETTER_NEGAALPHA
//#			define NEGASCOUT
//#			define OPEN_THEORY_NEGAALPHA
#endif
#endif
#endif
#endif
#endif


#ifndef COUNT_NODES_ALL
/** 全ノード数をカウントする（ルートノードを含む） */
#			define COUNT_NODES_ALL
#endif
#ifndef COUNT_NODES_TERM
/** 終端ノード数をカウントする(試合が終わっているノード) */
//#			define COUNT_NODES_TERM
#endif
#ifndef COUNT_NODES_LIMITED
/** 途中で解析を終了したノード数をカウントする */
//#			define COUNT_NODES_LIMITED
#endif


/** 探索を途中で終了する限界を有効にするか
 * TODO バグが存在する
 */
#ifndef ENABLE_LIMITED
//#		define ENABLE_LIMITED
#endif


/** 探索する深さ
 *
 * 最適解を求めるなら最後、つまり SIZE_X*SIZE_Y-4 を利用する
 * 仮仕様なので触らないように。
 */
#ifndef STARTING_DEPTH
#			define STARTING_DEPTH (SIZE_X*SIZE_Y-4)
//#			define STARTING_DEPTH 0
//#			define STARTING_DEPTH 16
//#			define STARTING_DEPTH 100
#endif
#ifndef ENDING_DEPTH
#			define ENDING_DEPTH (SIZE_X*SIZE_Y-4)
//#			define ENDING_DEPTH (STARTING_DEPTH)
//#			define ENDING_DEPTH 12
#endif


////// デバッグ用

/** 探索中のボードを表示するか(処理時間がかなり増える) */
#ifndef VIEW_BOARD
//#			define VIEW_BOARD
#endif

/** moblist を表示するか */
#ifndef VIEW_MOBLIST
//#		define VIEW_MOBLIST
#endif

/** 最適手の棋譜を表示するか */
#ifndef VIEW_KIFU
//#			define VIEW_KIFU
#endif

/** 最適手の棋譜をボードで表示するか */
#ifndef VIEW_KIFU_BOARD
//#			define VIEW_KIFU_BOARD
#endif

/** 初期ボードを与えるモード */
#ifndef INIT_BOARD
//#define INIT_BOARD
#endif

/** はじめの一手だけを出力する */
#ifndef VIEW_FIRST_MOB
//#define VIEW_FIRST_MOB
#endif

/** キャッシュモードを有効にする
 * TODO バグが存在する
 */
#ifndef ENABLE_CACHE
//#define ENABLE_CACHE
#endif

/** 枝刈りの質を記録する */
#ifndef RECORD_PRUNING_QUALITY
//#define RECORD_PRUNING_QUALITY
#endif

/** 完全プレイの速度などはCSV形式で出力する */
#ifndef VIEW_RESULT_BY_CSV
//#define VIEW_RESULT_BY_CSV
#endif


// 設定が正しいか検証する
#include "validator.h"


