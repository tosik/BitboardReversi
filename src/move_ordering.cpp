/** @file
 * @brief	Move Ordering評価関数
 *
 * @author	Toshiyuki Hirooka
 *
 */

#include "move_ordering.h"
#include <math.h>
#include <cstdlib>

using namespace Fujitalab::Reversi;

/** 開放度を取得
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @return	開放度
 */
int getOpenValue(Bitboard mobility, BitboardReversi *o, int depth)
{
	static bool my_pass_flag = false;
	if(mobility==0)
	{
		if(my_pass_flag)
		{
			my_pass_flag = false;
			return 0;
		}
		else
		{
			my_pass_flag = true;
			o->put(0);
			int e = -getOpenValue(mobility, o, depth+1);
			o->undo();
			my_pass_flag = false;
			return e;
		}
	}
	else
		my_pass_flag = false;

	int min = 1000000; // ∞
	Bitboard blank= ~(o->black | o->white);

	// 置ける場所を順に処理
	while(mobility!=0)
	{
		Bitboard m = getFirstBit(mobility);

		// 開放度を演算(相手の場合はネガティブにする)
		Bitboard r = o->getReversedPattern(m);
		int open =
			count(transferLeft(r)		& blank) +
			count(transferRight(r)		& blank) +
			count(transferTop(r)		& blank) +
			count(transferBottom(r)		& blank) +
			count(transferLeftTop(r)	& blank) +
			count(transferLeftBottom(r)	& blank) +
			count(transferRightTop(r)	& blank) +
			count(transferRightBottom(r)& blank);

		if(depth != 0)
		{
			o->put(m);
			open -= getOpenValue(o->getMobility(), o, depth-1);
			o->undo();
		}

		// 最小を計算
		if ( min > open )
			min = open;

		mobility ^= m;
	}

	my_pass_flag = false;
	return min;
}

/** 開放度理論に基づいて優秀なビットを取得
 *
 * [TODO] Potential Mobility という名前で知られている様子
 * 詳細は potential mobility reversi 等でGoogle検索!(全言語)
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ（2以上で発展的開放度理論）
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
Bitboard getBitByOpenTheory(Bitboard mobility, BitboardReversi *o, int depth)
{
	// [NOTE] このアルゴリズムにネガアルファは使用できない（末端以外のノードの数値も扱うため）

	// 着手可能な箇所が1つであれば、そこを返す
	if(count(mobility) == 1)
		return mobility;

	int min = 1000000; // ∞
	Bitboard dest = 0;
	Bitboard blank= ~(o->black | o->white);

	// 置ける場所を順に処理
	while(mobility!=0)
	{
		Bitboard m = getFirstBit(mobility);

		// 開放度を演算(相手の場合はネガティブにする)
		Bitboard r = o->getReversedPattern(m);
		int open =
			count(transferLeft(r)		& blank) +
			count(transferRight(r)		& blank) +
			count(transferTop(r)		& blank) +
			count(transferBottom(r)		& blank) +
			count(transferLeftTop(r)	& blank) +
			count(transferLeftBottom(r)	& blank) +
			count(transferRightTop(r)	& blank) +
			count(transferRightBottom(r)& blank);

		if(depth != 0)
		{
			o->put(m);
			open -= getOpenValue(o->getMobility(), o, depth-1);
			o->undo();
		}

		// 最小を計算
		if ( min > open )
		{
			min = open;
			dest = m;
		}

		mobility ^= m;
	}

	return dest;
}


/** ネガアルファにより評価値を取得する
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @param	alpha	αカット用変数
 * @param	beta	βカット用変数
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
int getValueByNegaalpha(Bitboard mobility, BitboardReversi *o, int depth, int alpha, int beta)
{
	static bool my_pass_flag = false;
	if(depth <= 0)
		return count(o->black) - count(o->white);

	if(mobility==0)
	{
		if(my_pass_flag)
		{
			my_pass_flag = false;
			return count(o->black) - count(o->white);
		}
		else
		{
			my_pass_flag = true;
			o->put(0);
			int e = -getValueByNegaalpha(mobility, o, depth+1, -beta, -alpha);
			o->undo();
			my_pass_flag = false;
			return e;
		}
	}
	else
		my_pass_flag = false;

	while(mobility!=0)
	{
		Bitboard m = getFirstBit(mobility);
		o->put(m);
		int e = -getValueByNegaalpha(o->getMobility(), o,  depth-1, -beta, -alpha);
		if ( e > alpha )
			alpha = e;
		o->undo();
		mobility ^= m;

		if ( alpha >= beta )
		{
			my_pass_flag = false;
			return alpha;
		}
	}
	my_pass_flag = false;
	return alpha;
}

/** ネガアルファを用いて最も優秀な手を取得する
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
Bitboard getBitByNegaalpha(Bitboard mobility, BitboardReversi *o, int depth)
{
	// 着手可能な箇所が1つであれば、そこを返す
	if(count(mobility) == 1)
		return mobility;

	Bitboard dest = 0;
	int alpha = -1000;
	int beta = 1000;

	while(mobility!=0)
	{
		Bitboard m = getFirstBit(mobility);
		o->put(m);
		int e = -getValueByNegaalpha(o->getMobility(), o,  depth-1, -beta, -alpha);
		if ( e > alpha )
		{
			dest = m;
			alpha = e;
		}
		o->undo();
		mobility ^= m;

		if ( alpha >= beta )
			return dest;
	}
	return dest;
}

/** 期待値法に基づいて期待値を取得する
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @param	alpha	αカット用変数
 * @param	beta	βカット用変数
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
float getExpectation(Bitboard mobility, BitboardReversi *o, int depth, float alpha, float beta)
{
	if(mobility == 0)
		return count(o->black) - count(o->white);

	// 期待値の演算
	if(depth == 0)
	{
		int h[64];
		int k = 0;
		// 子ノードの評価値を取得する(ついでにモンテカルロ法で用いる個数をkでカウントしている)
		while(mobility!=0)
		{
			Bitboard m = getFirstBit(mobility);
			o->put(m);
			h[k++] = count(o->black) - count(o->white);
			o->undo();
			mobility ^= m;
		}
		// 期待値をモンテカルロ法を用いて計算する
		float value = 0;
		const int cnt = 100;
		for(int j=0; j<cnt/2; j++)
		{
			float max = 0;
			for(int i=0; i<k; i++)
			{
				// 平均をh[i]、分散を1 とした正規分布に従った乱数を生成
				// 参照 : http://school-crc.kek.jp/SummerSchool06/HTML/practice0.htm
				float t = sqrt( -2.0 * log( 1 - (float)rand() / RAND_MAX ));
				float u = 2 * 3.14159265358979 * (float)rand() / RAND_MAX;
				float r1 = t * cos(u) + h[i];
				float r2 = t * sin(u) + h[i];

				// 最大値を求める
				if(r1 > max)
					max = r1;
				if(r2 > max)
					max = r2;
			}
			value += max / cnt;
		}
		return value;
	}

	while(mobility!=0)
	{
		Bitboard m = getFirstBit(mobility);
		o->put(m);
		float e = -getExpectation(o->getMobility(), o,  depth-1, -beta, -alpha);
		if ( e > alpha )
			alpha = e;
		o->undo();
		mobility ^= m;

		if ( alpha >= beta )
			return alpha;
	}
	return alpha;
}

/** 期待値法に基づいて最も優秀な手を取得する
 * 期待値法は http://ci.nii.ac.jp/naid/110002771150/ を用いている
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
Bitboard getBitByExpectationMethod(Bitboard mobility, BitboardReversi *o, int depth)
{
	// 着手可能な箇所が1つであれば、そこを返す
	if(count(mobility) == 1)
		return mobility;

	Bitboard dest = 0;
	float alpha = -1000;
	float beta = 1000;

	while(mobility!=0)
	{
		Bitboard m = getFirstBit(mobility);
		o->put(m);
		float e = -getExpectation(o->getMobility(), o,  depth-1, -beta, -alpha);
		if ( e > alpha )
		{
			dest = m;
			alpha = e;
		}
		o->undo();
		mobility ^= m;
		if ( alpha >= beta )
			return dest;
	}
	return dest;
}

/** N手先の着手可能な手数が最小のものを取得
 *
 * @param	mobility	着手可能場所のビットボード
 * @param	o	探索中のリバーシのオブジェクトへのポインタ
 * @param	depth	探索する深さ
 * @return	最も優秀な着手可能な場所のビットだけが1のビットボード
 */
int getMNM(Bitboard mobility, BitboardReversi *o, int depth)
{
	if(depth <= 0)
		return count(mobility);

	static bool my_pass_flag = false;
	if(depth <= 0)
		return count(o->black) - count(o->white);

	if(mobility==0)
	{
		if(my_pass_flag)
		{
			my_pass_flag = false;
			return count(mobility);
		}
		else
		{
			my_pass_flag = true;
			o->put(0);
			int e = -getMNM(mobility, o, depth+1);
			o->undo();
			my_pass_flag = false;
			return e;
		}
	}
	else
		my_pass_flag = false;

	int dest = 0;
	int self_nodes = count(mobility);

	while(mobility!=0)
	{
		Bitboard m = getFirstBit(mobility);
		o->put(m);
		dest += getMNM(o->getMobility(), o,  depth-1);
		o->undo();
		mobility ^= m;
	}
	return dest * self_nodes;
}

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
Bitboard getBitByMNM(Bitboard mobility, BitboardReversi *o, int depth)
{
	// 着手可能な箇所が1つであれば、そこを返す
	if(count(mobility) == 1)
		return mobility;

	Bitboard dest = 0;
	int min = 1000000;

	while(mobility!=0)
	{
		Bitboard m = getFirstBit(mobility);
		o->put(m);
		int e = getMNM(o->getMobility(), o,  depth-1);

		// [TODO] min は最小でも0なので、0が出現した時点でreturnできる。
		if ( e == 0 )
		{
			o->undo();
			return m;
		}
		if ( e < min )
		{
			dest = m;
			min = e;
		}
		o->undo();
		mobility ^= m;
	}
	return dest;
}

/** ランダムで手を決定
 *
 * @param	mobility	着手可能場所のビットボード
 * @return	ランダムで選択された着手可能な場所のビットだけが1のビットボード
 */
Bitboard getBitByRandom(Bitboard mobility)
{
	int n = count(mobility);
	// 着手可能な箇所が1つであれば、そこを返す
	if(n == 1)
		return mobility;
	if(n == 0)
		return 0;

	for(int i=0; i< rand()%(n-1); i++)
	   mobility ^= getFirstBit(mobility);
	return getFirstBit(mobility);
}

// テーブルから最も値の高いビットを取得
// table.rb から自動生成
Bitboard getBitByTable(Bitboard bits)
{
#include "tables"
	return 0;
}
