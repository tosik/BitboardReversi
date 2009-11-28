/** @file
 * @brief	最適手を探索したり、時間を計測する
 *
 * @author	P-04148 廣岡 聡之
 *
 */


/** @mainpage Bitboard Reversi の使い方
 *
 * \section introduction 紹介
 *
 * 本プログラムは、リバーシにおいて両者が最適手のみを着手する『完全プレイ』を行った場合に
 * ゲームの進行はどのようになるのかという疑問を解くために作られた。しかし、リバーシの最適手を求めるには
 * 天文学的時間がかかると言われており、我々が完全プレイの棋譜を知ることは非現実的である。
 * そのため、いかにして探索時間を削減するかが目的を達成するための重要事項である。
 *
 * 本プログラムでは、通常のリバーシの8x8の盤面でプレイするところを、4x4,4x5,5x5,5x6,...,8x8のサイズの様々な
 * 盤面で解析ができるようになっている。サイズが小さければ解析時間も短縮でき、6x6であれば現実的な時間で求めることも可能である。
 *
 * リバーシの解析はミニマックス法というゲーム木探索手法を用いており、さらにそれを改良したアルファベータ法がある。
 * 本プログラムではその2種類の探索法及び、それらをさらに改良した手法を用いて処理することができる。
 *
 * 改良版の中に探索優先順位を決定する Move Ordering 評価関数があるが、これはアルファベータの枝刈り品質（上位ノードで枝刈りされるほど良い）を
 * 向上するためのものである。基本的には、強い手を優先するだけではなくそれを非常に高速に決定する必要がある。
 *
 *
 * \section compileandrun コンパイルと実行
 *
 * 本プログラムのディレクトリで make コマンドを実行する。
 *
 * \code
 * make
 * \endcode
 *
 * 完了するとミニマックス法による解析を行う bbnegamax という実行ファイルが bin ディレクトリ下に生成される。
 * これを実行すると解析が始まり、結果を吐き出す。
 *
 * \code
 * bin/bbnegamax
 * \endcode
 *
 * この解析結果は make 時の設定によって違ってくる。
 * 通常は make コマンドで設定を行うこと。
 *
 * \code
 * make SIZE_X=5 SIZE_Y=5
 * \endcode
 *
 * とすると、ボードサイズ5x5でコンパイルされる。
 * なお、デフォルトではボードサイズ4x4でコンパイルされる。
 *
 * また、Rubyスクリプトを用いてバッチ処理も行える。scripts/run_algorithm_definition.rb を実行すると、
 * 指定したアルゴリズムを順次 make/実行 し、logs ディレクトリにログを出力する。
 *
 * \code
 * ruby scripts/run_algorithm_definition.rb
 * \endcode
 *
 * 指定方法はscripts/run_algorithm_definition.rb内を参照すること。
 *
 * \section commands 開発・実行等に必要なコマンド
 * gcc (g++), make, doxygen, ruby, svn
 *
 * なお、全てのコマンドはCygwinのインストーラsetup.exeからインストールできる。
 *
 */

////////////////////////////////////////////////////////////////
// ファイル読み込み
////////////////////////////////////////////////////////////////

// 標準ライブラリの読み込み
#include <iostream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>

// 設定ファイルの読み込み
#include "setting.h"

// ビットボード用リバーシクラスライブラリの読み込み
#include "bitboard_reversi.h"

// Move Ordering 用評価関数の読み込み
#include "move_ordering.h"

// 名前空間の使用を宣言
using namespace Fujitalab::Reversi;
using namespace std;

// インクルードの位置はここでないとだめ
// math.h内で log をマクロ定義しているためと思われる
#include <math.h>


/** 定数の文字列化用マクロ
 * 若干バッドノウハウ的手法
 *
 * 例えば、\#define STRING HELLO と定義された定数があるとき、
 * tostr(STRING) とすると、 "HELLO" という文字列として扱うことができる
 */
#define tostr(s) tostr_(s)
#define tostr_(s) #s

////////////////////////////////////////////////////////////////
// 型の定義
////////////////////////////////////////////////////////////////
/** ノード型
 *
 * ある1ノードの状態を保持する。
 * ただし、ボードの状態や親子・兄弟などのほかのノードとの関係は保持しない
 * [TODO] 限定的に使っているので、汎用的なNodeという名前は不適切ゆえ、名前と説明を適切なものに変更する
 */
struct Node
{
	/** ノードの評価値 */
	int value;

	/** そのノードで着手した手 */
	Bitboard mob;

	/** 初期化 */
	Node(int value = 0, Bitboard mob = 0)
	{
		this->value = value;
		this->mob = mob;
	}
};

////////////////////////////////////////////////////////////////
// グローバル変数の定義
////////////////////////////////////////////////////////////////

/** 全ノード数をカウントする変数 */
unsigned long long int count_nodes_all;

/** 終端ノード数をカウントする変数 */
unsigned long long int count_nodes_term;

/** 深さ制限により中断したノード数をカウントする変数 */
unsigned long long int count_nodes_limited;

/** パスしたかどうか(2連続パスを検出するために使う) */
bool pass_flag = false;

/** depth の限界値 */
const int MAX_DEPTH = ENDING_DEPTH * 2;

/** 最適ルート処理用 */
Node moblist[MAX_DEPTH][MAX_DEPTH];

#ifdef INIT_BOARD
/** 初期ボードを用いた際に指定した色 */
bool start_color;
/** 初期ボードを用いた際に指定したボードの状態 */
Bitboard b1, b2;
#endif

#ifdef ENABLE_LIMITED
/** パスの数をカウントする変数 */
int pass_count;
#endif

#ifdef RECORD_PRUNING_QUALITY
	int pruning_quality[MAX_DEPTH];
	int pruning_quality_max[MAX_DEPTH];
#endif

////////////////////////////////////////////////////////////////
// 関数の定義
////////////////////////////////////////////////////////////////

/** ボードの評価値を黒基準で求める
 *
 * @param	o	評価値を見たいリバーシのオブジェクトへのポインタ
 * @return	黒にとっての評価値
 */
int eval_for_black(BitboardReversi *o)
{
  return count(o->black) - count(o->white);
}

/** ボードの評価値を求める
 *
 * depthが奇数のときは黒,偶数のときは白を基準
 * @param	o	評価値を見たいリバーシのオブジェクトへのポインタ
 * @param	depth	探索中の深さ（つまり現在の色といえる）
 * @return	評価値
 */
int eval(BitboardReversi *o, int depth)
{
#ifdef INIT_BOARD
	if(start_color)
		return (count(o->black) - count(o->white)) * (depth % 2 == 0 ? -1 : 1);
	else
		return -(count(o->black) - count(o->white)) * (depth % 2 == 0 ? -1 : 1);
#else
  return (count(o->black) - count(o->white)) * (depth % 2 == 0 ? -1 : 1);
//  return (count(o->black) > count(o->white) ? 1 : 0) * (depth % 2 == 0 ? -1 : 1); // 必勝 
#endif
}

/** moblist の中身を表示
 *
 * デバッグ用途 に moblist を表示する
 */
void viewMobList()
{
	for ( int j = 0 ; j < MAX_DEPTH ; j ++ )
	{
		for ( int i = 0 ; i < MAX_DEPTH ; i ++ )
		{
			int v = moblist[i][j].value;
			if(v==-99)
				cout << "    ";
			else if(moblist[i][j].mob == 0)
				cout << setw(3) << v << "*";
			else
				cout << setw(3) << v << " ";
//            Bitboard b = moblist[i][j].mob;
//            if ( b == 0xffffffffffffffffULL)
//                cout << "   ";
//            else
//                cout << setw(2) << (char)(log2(b) +'0') << " ";
		}
		cout << endl;
	}
	cout << endl;
}

/** moblist を更新する
 *
 * moblist とは最適手を求める際に用いる変数
 * 探索中に評価値を求めたら次々に moblist に更新を行うことで
 * 求めることができる。
 * @param	node	更新したいノード
 * @param	depth	その深さ
 */
void updateMobList(Node node, int depth)
{
#ifdef VIEW_MOBLIST
	cout << "depth: " << depth << endl;
	cout << "mob: " << node.mob << endl;
	cout << "value: " << node.value << endl;
#endif

	// 前に探索した場所より大きければ
	if ( node.value > moblist[depth][depth].value )
	{
		// moblist を更新する
		moblist[depth][depth + 1] = node;

		// シフトする(for文を展開すると高速になるかも)
		for ( int i = depth ; i < MAX_DEPTH ; i ++ )
			moblist[i][depth] = moblist[i][depth + 1];
	}

	// 作業領域を消去
	for ( int j = depth + 1 ; j < MAX_DEPTH ; j ++ )
	{
		for ( int i = j - 1 ; i < MAX_DEPTH ; i ++ )
		{
			moblist[i][j].value = -99;
#ifdef VIEW_MOBLIST // デバッグ用 表示する際に必要
			moblist[i][j].mob = 0xffffffffffffffffULL;
#endif
		}
	}

#ifdef VIEW_MOBLIST
	viewMobList();
#endif

}



////////////////////////////////////////////////////////////////
// ゲーム木探索関数の定義
////////////////////////////////////////////////////////////////

/** ネガマックス法を用いたゲーム木探索
 *
 * 最適手は、探索後の moblist の中身を見ること
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	現在の探索中の深さ
 * @param	depth_limited	探索を途中で打ち切るための限界深さ
 * @return	探索結果の評価値
 */
int negamax(BitboardReversi *o, int depth, int depth_limited)
{
#ifdef COUNT_NODES_ALL
	count_nodes_all ++;
#endif

#ifdef ENABLE_LIMITED
	if(depth == depth_limited + pass_count)
	{
#ifdef COUNT_NODES_TERM
		count_nodes_limited ++;
#endif
		int e = eval(o,depth);
//        updateMobList(Node(e,0), depth);
#ifdef VIEW_BOARD
		o->view();
#endif
		pass_flag = false;
		return e;
	}
#endif

	Bitboard mobility = o->getMobility();
	if(mobility==0)
	{
#ifdef COUNT_NODES_ALL
		count_nodes_all --;
#endif
#ifdef ENABLE_LIMITED
		pass_count ++;
#endif
		if(pass_flag)
		{
#ifdef COUNT_NODES_ALL
			count_nodes_all ++;
#endif
#ifdef COUNT_NODES_TERM
			count_nodes_term ++;
#endif
			int e = eval(o,depth);
//            updateMobList(Node(e,0), depth);
#ifdef VIEW_BOARD
			o->view();
			cout << "value: " << e << endl;
#endif
			pass_flag = false;
#ifdef ENABLE_LIMITED
			pass_count --;
#endif
			return e;
		}
		else
		{
			pass_flag = true;
			o->put(0);
			int e = -negamax(o, depth+1, depth_limited);
			updateMobList(Node(e,0), depth);
#ifdef VIEW_BOARD
			o->view();
			cout << "value: " << e << endl;
#endif
			o->undo();
			pass_flag = false;
#ifdef ENABLE_LIMITED
			pass_count --;
#endif
			return e;
		}
	}
	else
	{
		pass_flag = false;
	}
	while(mobility!=0)
	{
		Bitboard m = getFirstBit(mobility);
		o->put(m);
		int e = -negamax(o, depth+1, depth_limited);
		updateMobList(Node(e,m), depth);
#ifdef VIEW_BOARD
		o->view();
		cout << "value: " << e << endl;
#endif
		o->undo();
		mobility ^= m;
	}
	pass_flag = false;
	return moblist[depth][depth].value;
}

/** ネガアルファ法を用いたゲーム木探索
 *
 * 最適手は、探索後の moblist の中身を見ること
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	現在の探索中の深さ
 * @param	depth_limited	探索を途中で打ち切るための限界深さ
 * @param	alpha	探索中に用いる内部的変数
 * @param	beta	探索中に用いる内部的変数
 * @return	探索結果の評価値
 */
int negaalpha(BitboardReversi *o, int depth, int depth_limited, int alpha, int beta)
{
#ifdef COUNT_NODES_ALL
	count_nodes_all ++;
#endif


#ifdef ENABLE_LIMITED
	if(depth == depth_limited + pass_count)
	{
#ifdef COUNT_NODES_TERM
		count_nodes_limited ++;
#endif
		int e = eval(o,depth);
		// 無くてよい
//        updateMobList(Node(e,0), depth);
#ifdef VIEW_BOARD
		o->view();
#endif
		pass_flag = false;
		return e;
	}
#endif

	Bitboard mobility = o->getMobility();
	if(mobility==0)
	{
#ifdef COUNT_NODES_ALL
		count_nodes_all --;
#endif
#ifdef ENABLE_LIMITED
		pass_count ++;
#endif
		if(pass_flag)
		{
#ifdef COUNT_NODES_ALL
			count_nodes_all ++;
#endif
#ifdef COUNT_NODES_TERM
			count_nodes_term ++;
#endif
			int e = eval(o,depth);
			// 無くてよい
//            updateMobList(Node(e,0), depth);
#ifdef VIEW_BOARD
			o->view();
#endif
			pass_flag = false;
#ifdef ENABLE_LIMITED
			pass_count --;
#endif
			return e;
		}
		else
		{
			pass_flag = true;
			o->put(0);
			int e = -negaalpha(o, depth+1, depth_limited, -beta, -alpha);
			updateMobList(Node(e,0), depth);
#ifdef VIEW_BOARD
			o->view();
#endif
			o->undo();
			pass_flag = false;
#ifdef ENABLE_LIMITED
			pass_count --;
#endif
			return e;
		}
	}
	else
		pass_flag = false;

#ifdef RECORD_PRUNING_QUALITY
//            pruning_quality_max[depth] ++;
			pruning_quality_max[depth] += count(mobility);
#endif
	while(mobility!=0)
	{

		// 優先順位を決定するアルゴリズムの選択
#ifdef ALGORITHM_DEFINITION		// 外部の定義ファイルを使用する
#		include "algorithm_definition"
#else
#ifdef BETTER_NEGAALPHA
		Bitboard m = getBitByTable(mobility);
#else
#ifdef OPEN_THEORY_NEGAALPHA
		Bitboard m = getBitByOpenTheory(mobility,o,1);
#else
#ifdef EXPECTATION_METHOD_NEGAALPHA
		Bitboard m = getBitByExpectationMethod(mobility,o,MAX_DEPTH); // 枝刈り率がすごく高い
#else
#ifdef VALUE_BY_NEGAALPHA_NEGAALPHA
		Bitboard m = getBitByNegaalpha(mobility,o, MAX_DEPTH); // 枝刈り率がかなり良い
#else
#ifdef MNM_NEGAALPHA
		Bitboard m = getBitByMNM(mobility,o, 1);
#else
#ifdef RANDOM_NEGAALPHA
		Bitboard m = getBitByRandom(mobility);
#else
		Bitboard m = getFirstBit(mobility);
#endif
#endif
#endif
#endif
#endif
#endif
#endif

		o->put(m);
		int e = -negaalpha(o, depth+1, depth_limited, -beta, -alpha);
		if ( e > alpha )
			alpha = e;
		updateMobList(Node(e,m), depth);
#ifdef VIEW_BOARD
		o->view();
#endif
		o->undo();
		mobility ^= m;

		// alpha beta cut
		if ( alpha >= beta )
		{
#ifdef RECORD_PRUNING_QUALITY
//            pruning_quality[depth] ++;
			pruning_quality[depth] += count(mobility);
#endif
			pass_flag = false;
			return alpha;
		}
	}
	pass_flag = false;
	return alpha;
}


////////////////////////////////////////////////////////////////
// 探索アルゴリズム選択
////////////////////////////////////////////////////////////////

/** ゲーム木を探索して最適手を求める */
void perfect(int argc, char *argv[])
{
	// リバーシクラスのオブジェクト
	BitboardReversi o;

#ifdef	INIT_BOARD
	// 初期ボードを設定する
	// 初期ボードの仕様
	// ./bbreversi BLACK WHITE COLOR
	// というコマンドを入力すると、BLACK, WHITEを初期ボードとし、COLORを開始色として処理する
	if(argc == 4)
	{
		string s1(argv[1]);
		istringstream iss1(s1);
		iss1 >> b1;
		string s2(argv[2]);
		istringstream iss2(s2);
		iss2 >> b2;
		start_color = atoi(argv[3]) == 0 ? true : false;
		o.setBoard(b1,b2,true);
		if(!start_color)
			o.put(0);
	}
	else
	{
		cout << "fatal arguments" << endl;
		cout << "argc = " << argc << endl;
		for(int i=0; i<argc; i++)
			cout << "argv[" << i << "] = " << argv[i] << endl;
		return;
	}
#endif

	// 演算開始時間を記録
	clock_t time_start = clock();

#ifdef NEGAMAX
	negamax(&o, 0, ENDING_DEPTH);
#else
	negaalpha(&o, 0, ENDING_DEPTH, -1000, 1000);
#endif

	// 演算終了時間を記録
	clock_t time_end = clock();

	double t = (time_end - time_start)/(double)CLOCKS_PER_SEC*1000;
#ifndef VIEW_RESULT_BY_CSV
	cout << "time = " << t << "ms" << " (" << t/1000/60/60 << "hours) " << endl;
#ifdef COUNT_NODES_ALL
	cout << "nodes = " << count_nodes_all << endl;
	cout << "time / nodes = " << ( t / count_nodes_all ) << endl;
#endif
#ifdef COUNT_NODES_TERM
	cout << "nodes(term) = " << count_nodes_term << endl;
#endif
#ifdef COUNT_NODES_LIMITED
	cout << "nodes(limited) = " << count_nodes_limited << endl;
#endif
#else
	cout << tostr(ALGORITHM_NAME) << "," << SIZE_X << "," << SIZE_Y << "," << DEPTH_START << "," << DEPTH_END << "," << count_nodes_all << "," << t <<  "," << ( t / count_nodes_all ) << endl;
#endif

#ifdef RECORD_PRUNING_QUALITY
	cout << "Pruning Quality" << endl;
	for(int i=0; i<MAX_DEPTH; i++)
	{
		cout << "depth = " << i << "\t : " << pruning_quality[i] << " / " << pruning_quality_max[i];
		if(pruning_quality_max[i] != 0)
			cout << "\t = "  << (float)pruning_quality[i] / (float)pruning_quality_max[i] << endl;
		else
			cout << "\t = 0" << endl;
	}
#endif

	// 取得した moblist を使って最適な手を再現する
	{
		BitboardReversi bbr;

#ifdef	INIT_BOARD
		// 初期ボードを設定する
		bbr.setBoard(b1,b2,true);
		if(!start_color)
			bbr.put(0);
#endif

#ifndef	VIEW_FIRST_MOB
		int i = 0;
		while ( moblist[i][0].value != -99 )
		{
			// moblistからノードを順に取得
			Node n = moblist[i++][0];

			// パス以外なら表示
			if ( n.mob != 0 )
			{
				// 棋譜を表示
#ifdef	VIEW_KIFU
				cout << MAX_SIZE_X-(log2(n.mob)-1)%MAX_SIZE_Y-1 << "-" << MAX_SIZE_Y-(log2(n.mob)-1)/MAX_SIZE_X-1 << endl;
#endif
#ifdef	VIEW_KIFU_BOARD
				bbr.view();
#endif
			}

			// 最適手再現用リバーシに着手する
			bbr.putSafety(n.mob);
#ifdef	VIEW_KIFU_BOARD
			if ( n.mob != 0 )
				cout << "value: " << n.value << endl;
#endif
		}
#ifdef	VIEW_KIFU_BOARD
		bbr.view();
#endif
#else
		Node n = moblist[0][0];
		bbr.putSafety(n.mob);
		// 黒から開始されていたら、戻り値は白なので、反転して白を返す
		// 白から開始されていたら、反転せず黒で返す
		if(start_color)
			cout << bbr.white << " " << bbr.black << " 1" << endl;
		else
			cout << bbr.black << " " << bbr.white << " 0" << endl;
		cout << "value : " << n.value << endl;
//        bbr.current_color ^= !start_color;
		bbr.view();
#endif

	}
}


/** エントリーポイント */
int main(int argc, char *argv[])
{
	// 乱数シードをランダマイズ
	srand((unsigned)time(NULL));

	// moblist を初期化
	for ( int j = 0 ; j < MAX_DEPTH; j ++ )
	{
		for ( int i = 0 ; i < MAX_DEPTH; i ++ )
		{
			moblist[i][j].value = -99;
			moblist[i][j].mob = 0xffffffffffffffffULL;
		}
	}

#ifdef RECORD_PRUNING_QUALITY
	// 枝刈り質を記録する配列を初期化
	for(int i=0; i<MAX_DEPTH; i++)
		pruning_quality[MAX_DEPTH] = pruning_quality_max[MAX_DEPTH] = 0;
#endif


#ifndef VIEW_RESULT_BY_CSV

// アルゴリズムの名前を表示する
#ifndef ALGORITHM_DEFINITION
#ifndef	VIEW_FIRST_MOB
	cout << "algorithm is ";
#ifdef NEGAMAX
	cout << "negamax" << endl;
#else
#ifdef NEGAALPHA
	cout << "negaalpha" << endl;
#else
#ifdef BETTER_NEGAALPHA
	cout << "better_negaalpha" << endl;
#else
#ifdef OPEN_THEORY_NEGAALPHA
	cout << "open_theory_negaalpha" << endl;
#else
#ifdef EXPECTATION_METHOD_NEGAALPHA
	cout << "expectation_method_negaalpha" << endl;
#else
#ifdef VALUE_BY_NEGAALPHA_NEGAALPHA
	cout << "value_by_negaalpha_negaalpha" << endl;
#else
#ifdef MNM_NEGAALPHA
	cout << "mnm_negaalpha" << endl;
#else
#ifdef RANDOM_NEGAALPHA
	cout << "random_negaalpha" << endl;
#else
#ifdef NEGASCOUT
	cout << "negascout" << endl;
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

#ifndef	VIEW_FIRST_MOB
	cout << "size is (" << SIZE_X << "-" << SIZE_Y << ")" << endl;
#endif

#endif

	// 解析をする
	perfect(argc, argv);

#ifndef VIEW_RESULT_BY_CSV
	cout << "-----------------" << endl;
#endif

}


