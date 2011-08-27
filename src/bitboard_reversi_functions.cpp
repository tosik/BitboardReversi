/** @file
 * @brief	Bitboard Reversi �֐��̃R�[�h�t�@�C��
 *
 * @author	Toshiyuki Hirooka
 *
 */

#include "bitboard_reversi_functions.h"
using namespace Fujitalab::Reversi;

#include <iostream>
using namespace std;

//#include <mmintrin.h> 	// for MMX
//#include <xmmintrin.h>	// for SSE
//#include <emmintrin.h>	// for SSE2
//#include <pmmintrin.h>	// for SSE3
//#include <tmmintrin.h>	// for SSE4

namespace Fujitalab
{
	namespace Reversi
	{

		// 2����̃��K���Y�����擾����
		int log2(Bitboard b)
		{
			int n = 0;
			while ( b != 0 )
			{
				b /= 2;
				n ++;
			}
			return n;
		}

		// �r�b�g��̂����͂��߂ɏo������r�b�g�������擾
		Bitboard getFirstBit(Bitboard bits)
		{
			return bits ^ ( bits & ( bits - 1 ) );
		}

		// �r�b�g��� 1 �̐����擾
		int count(Bitboard n)
		{
			// ���Ԃ�A�I�[�_�� log2(bits-width) . 64bits �Ȃ̂� log2(64) = 7 ��œ������o����
			n = ((n>>1)  & 0x5555555555555555ULL) + (n & 0x5555555555555555ULL);
			n = ((n>>2)  & 0x3333333333333333ULL) + (n & 0x3333333333333333ULL);
			n = ((n>>4)  & 0x0f0f0f0f0f0f0f0fULL) + (n & 0x0f0f0f0f0f0f0f0fULL);
			n = ((n>>8)  & 0x00ff00ff00ff00ffULL) + (n & 0x00ff00ff00ff00ffULL);
			n = ((n>>16) & 0x0000ffff0000ffffULL) + (n & 0x0000ffff0000ffffULL);
			n = (n>>32) + (n & 0x00000000ffffffffULL);
			return (int)n;

		}

		// ���W�� Bitboard �ɂ���
		// �^�������W�̃r�b�g��1,���̑���0�̃r�b�g�{�[�h���擾
		Bitboard toBitboard(int x, int y)
		{
			return ( 0x8000000000000000ULL >> ( y * MAX_SIZE_X + x ) );
		}

		// �r�b�g�{�[�h��\������
		void view(Bitboard b)
		{
			for ( int i = 0 ; i < MAX_SIZE ; i ++ )
			{
				// ���s
				if ( i % MAX_SIZE_X == 0 )
					cout << endl;

				// 1�̃r�b�g������
				if ( ( b & 0x8000000000000000ULL ) != 0 )
					cout << "1 ";
				else
					cout << "0 ";

				// ���̏ꏊ�ɐi��
				b <<= 1;

			}
			cout << endl;
		}
		// ��������ɔ��]�������r�b�g�{�[�h�Ŏ擾
		Bitboard getMobilityLeft(Bitboard black, Bitboard white)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			Bitboard w = white & 0x6060606000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			Bitboard w = white & 0x6060606060000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			Bitboard w = white & 0x7070707000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			Bitboard w = white & 0x7070707070000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			Bitboard w = white & 0x7070707070700000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			Bitboard w = white & 0x7878787878000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			Bitboard w = white & 0x7878787878780000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			Bitboard w = white & 0x7878787878787800ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			Bitboard w = white & 0x7c7c7c7c7c7c0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			Bitboard w = white & 0x7c7c7c7c7c7c7c00ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			Bitboard w = white & 0x7c7c7c7c7c7c7c7cULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			Bitboard w = white & 0x7e7e7e7e7e7e7e00ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			Bitboard w = white & 0x7e7e7e7e7e7e7e7eULL;
#endif
			Bitboard t = w & (black >> 1);
			t |= w & (t >> 1);
			t |= w & (t >> 1);
			t |= w & (t >> 1);
			t |= w & (t >> 1);
			t |= w & (t >> 1);
			Bitboard blank = ~(black | white);
			return blank & (t >> 1);
		}
		Bitboard getMobilityRight(Bitboard black, Bitboard white)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			Bitboard w = white & 0x6060606000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			Bitboard w = white & 0x6060606060000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			Bitboard w = white & 0x7070707000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			Bitboard w = white & 0x7070707070000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			Bitboard w = white & 0x7070707070700000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			Bitboard w = white & 0x7878787878000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			Bitboard w = white & 0x7878787878780000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			Bitboard w = white & 0x7878787878787800ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			Bitboard w = white & 0x7c7c7c7c7c7c0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			Bitboard w = white & 0x7c7c7c7c7c7c7c00ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			Bitboard w = white & 0x7c7c7c7c7c7c7c7cULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			Bitboard w = white & 0x7e7e7e7e7e7e7e00ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			Bitboard w = white & 0x7e7e7e7e7e7e7e7eULL;
#endif
			Bitboard t = w & (black << 1);
			t |= w & (t << 1);
			t |= w & (t << 1);
			t |= w & (t << 1);
			t |= w & (t << 1);
			t |= w & (t << 1);
			Bitboard blank = ~(black | white);
			return blank & (t << 1);
		}
		Bitboard getMobilityTop(Bitboard black, Bitboard white)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			Bitboard w = white & 0x00f0f00000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			Bitboard w = white & 0x00f0f0f000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			Bitboard w = white & 0x00f8f80000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			Bitboard w = white & 0x00f8f8f800000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			Bitboard w = white & 0x00f8f8f8f8000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			Bitboard w = white & 0x00fcfcfc00000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			Bitboard w = white & 0x00fcfcfcfc000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			Bitboard w = white & 0x00fcfcfcfcfc0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			Bitboard w = white & 0x00fefefefe000000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			Bitboard w = white & 0x00fefefefefe0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			Bitboard w = white & 0x00fefefefefefe00ULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			Bitboard w = white & 0x00ffffffffff0000ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			Bitboard w = white & 0x00ffffffffffff00ULL;
#endif
			Bitboard t = w & (black >> 8);
			t |= w & (t >> 8);
			t |= w & (t >> 8);
			t |= w & (t >> 8);
			t |= w & (t >> 8);
			t |= w & (t >> 8);
			Bitboard blank = ~(black | white);
			return blank & (t >> 8);
		}
		Bitboard getMobilityBottom(Bitboard black, Bitboard white)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			Bitboard w = white & 0x00f0f00000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			Bitboard w = white & 0x00f0f0f000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			Bitboard w = white & 0x00f8f80000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			Bitboard w = white & 0x00f8f8f800000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			Bitboard w = white & 0x00f8f8f8f8000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			Bitboard w = white & 0x00fcfcfc00000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			Bitboard w = white & 0x00fcfcfcfc000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			Bitboard w = white & 0x00fcfcfcfcfc0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			Bitboard w = white & 0x00fefefefe000000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			Bitboard w = white & 0x00fefefefefe0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			Bitboard w = white & 0x00fefefefefefe00ULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			Bitboard w = white & 0x00ffffffffff0000ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			Bitboard w = white & 0x00ffffffffffff00ULL;
#endif
			Bitboard t = w & (black << 8);
			t |= w & (t << 8);
			t |= w & (t << 8);
			t |= w & (t << 8);
			t |= w & (t << 8);
			t |= w & (t << 8);
			Bitboard blank = ~(black | white);
			return blank & (t << 8);
		}
		Bitboard getMobilityLeftTop(Bitboard black, Bitboard white)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			Bitboard w = white & 0x0060600000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			Bitboard w = white & 0x0060606000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			Bitboard w = white & 0x0070700000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			Bitboard w = white & 0x0070707000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			Bitboard w = white & 0x0070707070000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			Bitboard w = white & 0x0078787800000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			Bitboard w = white & 0x0078787878000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			Bitboard w = white & 0x0078787878780000ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			Bitboard w = white & 0x007c7c7c7c000000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			Bitboard w = white & 0x007c7c7c7c7c0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			Bitboard w = white & 0x007c7c7c7c7c7c00ULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			Bitboard w = white & 0x007e7e7e7e7e0000ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			Bitboard w = white & 0x007e7e7e7e7e7e00ULL;
#endif
			Bitboard t = w & (black >> 9);
			t |= w & (t >> 9);
			t |= w & (t >> 9);
			t |= w & (t >> 9);
			t |= w & (t >> 9);
			t |= w & (t >> 9);
			Bitboard blank = ~(black | white);
			return blank & (t >> 9);
		}
		Bitboard getMobilityLeftBottom(Bitboard black, Bitboard white)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			Bitboard w = white & 0x0060600000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			Bitboard w = white & 0x0060606000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			Bitboard w = white & 0x0070700000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			Bitboard w = white & 0x0070707000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			Bitboard w = white & 0x0070707070000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			Bitboard w = white & 0x0078787800000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			Bitboard w = white & 0x0078787878000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			Bitboard w = white & 0x0078787878780000ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			Bitboard w = white & 0x007c7c7c7c000000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			Bitboard w = white & 0x007c7c7c7c7c0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			Bitboard w = white & 0x007c7c7c7c7c7c00ULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			Bitboard w = white & 0x007e7e7e7e7e0000ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			Bitboard w = white & 0x007e7e7e7e7e7e00ULL;
#endif
			Bitboard t = w & (black << 7);
			t |= w & (t << 7);
			t |= w & (t << 7);
			t |= w & (t << 7);
			t |= w & (t << 7);
			t |= w & (t << 7);
			Bitboard blank = ~(black | white);
			return blank & (t << 7);
		}
		Bitboard getMobilityRightTop(Bitboard black, Bitboard white)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			Bitboard w = white & 0x0060600000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			Bitboard w = white & 0x0060606000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			Bitboard w = white & 0x0070700000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			Bitboard w = white & 0x0070707000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			Bitboard w = white & 0x0070707070000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			Bitboard w = white & 0x0078787800000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			Bitboard w = white & 0x0078787878000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			Bitboard w = white & 0x0078787878780000ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			Bitboard w = white & 0x007c7c7c7c000000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			Bitboard w = white & 0x007c7c7c7c7c0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			Bitboard w = white & 0x007c7c7c7c7c7c00ULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			Bitboard w = white & 0x007e7e7e7e7e0000ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			Bitboard w = white & 0x007e7e7e7e7e7e00ULL;
#endif
			Bitboard t = w & (black >> 7);
			t |= w & (t >> 7);
			t |= w & (t >> 7);
			t |= w & (t >> 7);
			t |= w & (t >> 7);
			t |= w & (t >> 7);
			Bitboard blank = ~(black | white);
			return blank & (t >> 7);
		}
		Bitboard getMobilityRightBottom(Bitboard black, Bitboard white)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			Bitboard w = white & 0x0060600000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			Bitboard w = white & 0x0060606000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			Bitboard w = white & 0x0070700000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			Bitboard w = white & 0x0070707000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			Bitboard w = white & 0x0070707070000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			Bitboard w = white & 0x0078787800000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			Bitboard w = white & 0x0078787878000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			Bitboard w = white & 0x0078787878780000ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			Bitboard w = white & 0x007c7c7c7c000000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			Bitboard w = white & 0x007c7c7c7c7c0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			Bitboard w = white & 0x007c7c7c7c7c7c00ULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			Bitboard w = white & 0x007e7e7e7e7e0000ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			Bitboard w = white & 0x007e7e7e7e7e7e00ULL;
#endif
			Bitboard t = w & (black << 9);
			t |= w & (t << 9);
			t |= w & (t << 9);
			t |= w & (t << 9);
			t |= w & (t << 9);
			t |= w & (t << 9);
			Bitboard blank = ~(black | white);
			return blank & (t << 9);
		}


		// 90�x��]
		Bitboard rotate(Bitboard x)
		{

			// 8x8 ��p (������������p���Ă��邽�߁A4x4, 2x2�ɂ����p�ł���)
			x =
				((x << 1) & 0xAA00AA00AA00AA00ULL) |
				((x >> 1) & 0x0055005500550055ULL) |
				((x >> 8) & 0x00AA00AA00AA00AAULL) |
				((x << 8) & 0x5500550055005500ULL) ;
			
			x =
				((x << 2) & 0xCCCC0000CCCC0000ULL) |
				((x >> 2) & 0x0000333300003333ULL) |
				((x >>16) & 0x0000CCCC0000CCCCULL) |
				((x <<16) & 0x3333000033330000ULL) ;
			
			x =
				((x << 4) & 0xF0F0F0F000000000ULL) |
				((x >> 4) & 0x000000000F0F0F0FULL) |
				((x >>32) & 0x00000000F0F0F0F0ULL) |
				((x <<32) & 0x0F0F0F0F00000000ULL) ;
			return x;
		}

		Bitboard transferLeft(Bitboard b)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			return ( b << 1 ) & 0xe0e0e0e000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			return ( b << 1 ) & 0xe0e0e0e0e0000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			return ( b << 1 ) & 0xf0f0f0f000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			return ( b << 1 ) & 0xf0f0f0f0f0000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			return ( b << 1 ) & 0xf0f0f0f0f0f00000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			return ( b << 1 ) & 0xf8f8f8f8f8000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			return ( b << 1 ) & 0xf8f8f8f8f8f80000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			return ( b << 1 ) & 0xf8f8f8f8f8f8f800ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			return ( b << 1 ) & 0xfcfcfcfcfcfc0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			return ( b << 1 ) & 0xfcfcfcfcfcfcfc00ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			return ( b << 1 ) & 0xfcfcfcfcfcfcfcfcULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			return ( b << 1 ) & 0xfefefefefefefe00ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			return ( b << 1 ) & 0xfefefefefefefefeULL;
#endif
		}
		Bitboard transferRight(Bitboard b)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			return ( b >> 1 ) & 0x7070707000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			return ( b >> 1 ) & 0x7070707070000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			return ( b >> 1 ) & 0x7878787800000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			return ( b >> 1 ) & 0x7878787878000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			return ( b >> 1 ) & 0x7878787878780000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			return ( b >> 1 ) & 0x7c7c7c7c7c000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			return ( b >> 1 ) & 0x7c7c7c7c7c7c0000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			return ( b >> 1 ) & 0x7c7c7c7c7c7c7c00ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			return ( b >> 1 ) & 0x7e7e7e7e7e7e0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			return ( b >> 1 ) & 0x7e7e7e7e7e7e7e00ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			return ( b >> 1 ) & 0x7e7e7e7e7e7e7e7eULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			return ( b >> 1 ) & 0x7f7f7f7f7f7f7f00ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			return ( b >> 1 ) & 0x7f7f7f7f7f7f7f7fULL;
#endif
		}
		Bitboard transferTop(Bitboard b)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			return ( b << 8 ) & 0xf0f0f00000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			return ( b << 8 ) & 0xf0f0f0f000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			return ( b << 8 ) & 0xf8f8f80000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			return ( b << 8 ) & 0xf8f8f8f800000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			return ( b << 8 ) & 0xf8f8f8f8f8000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			return ( b << 8 ) & 0xfcfcfcfc00000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			return ( b << 8 ) & 0xfcfcfcfcfc000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			return ( b << 8 ) & 0xfcfcfcfcfcfc0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			return ( b << 8 ) & 0xfefefefefe000000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			return ( b << 8 ) & 0xfefefefefefe0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			return ( b << 8 ) & 0xfefefefefefefe00ULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			return ( b << 8 ) & 0xffffffffffff0000ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			return ( b << 8 ) & 0xffffffffffffff00ULL;
#endif
		}
		Bitboard transferBottom(Bitboard b)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			return ( b >> 8 ) & 0x00f0f0f000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			return ( b >> 8 ) & 0x00f0f0f0f0000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			return ( b >> 8 ) & 0x00f8f8f800000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			return ( b >> 8 ) & 0x00f8f8f8f8000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			return ( b >> 8 ) & 0x00f8f8f8f8f80000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			return ( b >> 8 ) & 0x00fcfcfcfc000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			return ( b >> 8 ) & 0x00fcfcfcfcfc0000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			return ( b >> 8 ) & 0x00fcfcfcfcfcfc00ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			return ( b >> 8 ) & 0x00fefefefefe0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			return ( b >> 8 ) & 0x00fefefefefefe00ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			return ( b >> 8 ) & 0x00fefefefefefefeULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			return ( b >> 8 ) & 0x00ffffffffffff00ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			return ( b >> 8 ) & 0x00ffffffffffffffULL;
#endif
		}
		Bitboard transferRightBottom(Bitboard b)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			return ( b >> 9 ) & 0x0070707000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			return ( b >> 9 ) & 0x0070707070000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			return ( b >> 9 ) & 0x0078787800000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			return ( b >> 9 ) & 0x0078787878000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			return ( b >> 9 ) & 0x0078787878780000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			return ( b >> 9 ) & 0x007c7c7c7c000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			return ( b >> 9 ) & 0x007c7c7c7c7c0000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			return ( b >> 9 ) & 0x007c7c7c7c7c7c00ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			return ( b >> 9 ) & 0x007e7e7e7e7e0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			return ( b >> 9 ) & 0x007e7e7e7e7e7e00ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			return ( b >> 9 ) & 0x007e7e7e7e7e7e7eULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			return ( b >> 9 ) & 0x007f7f7f7f7f7f00ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			return ( b >> 9 ) & 0x007f7f7f7f7f7f7fULL;
#endif
		}
		Bitboard transferRightTop(Bitboard b)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			return ( b << 7 ) & 0x7070700000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			return ( b << 7 ) & 0x7070707000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			return ( b << 7 ) & 0x7878780000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			return ( b << 7 ) & 0x7878787800000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			return ( b << 7 ) & 0x7878787878000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			return ( b << 7 ) & 0x7c7c7c7c00000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			return ( b << 7 ) & 0x7c7c7c7c7c000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			return ( b << 7 ) & 0x7c7c7c7c7c7c0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			return ( b << 7 ) & 0x7e7e7e7e7e000000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			return ( b << 7 ) & 0x7e7e7e7e7e7e0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			return ( b << 7 ) & 0x7e7e7e7e7e7e7e00ULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			return ( b << 7 ) & 0x7f7f7f7f7f7f0000ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			return ( b << 7 ) & 0x7f7f7f7f7f7f7f00ULL;
#endif
		}
		Bitboard transferLeftBottom(Bitboard b)
		{
#if SIZE_X == 4 && SIZE_Y == 4
			return ( b >> 7 ) & 0x00e0e0e000000000ULL;
#elif SIZE_X == 4 && SIZE_Y == 5
			return ( b >> 7 ) & 0x00e0e0e0e0000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 4
			return ( b >> 7 ) & 0x00f0f0f000000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 5
			return ( b >> 7 ) & 0x00f0f0f0f0000000ULL;
#elif SIZE_X == 5 && SIZE_Y == 6
			return ( b >> 7 ) & 0x00f0f0f0f0f00000ULL;
#elif SIZE_X == 6 && SIZE_Y == 5
			return ( b >> 7 ) & 0x00f8f8f8f8000000ULL;
#elif SIZE_X == 6 && SIZE_Y == 6
			return ( b >> 7 ) & 0x00f8f8f8f8f80000ULL;
#elif SIZE_X == 6 && SIZE_Y == 7
			return ( b >> 7 ) & 0x00f8f8f8f8f8f800ULL;
#elif SIZE_X == 7 && SIZE_Y == 6
			return ( b >> 7 ) & 0x00fcfcfcfcfc0000ULL;
#elif SIZE_X == 7 && SIZE_Y == 7
			return ( b >> 7 ) & 0x00fcfcfcfcfcfc00ULL;
#elif SIZE_X == 7 && SIZE_Y == 8
			return ( b >> 7 ) & 0x00fcfcfcfcfcfcfcULL;
#elif SIZE_X == 8 && SIZE_Y == 7
			return ( b >> 7 ) & 0x00fefefefefefe00ULL;
#elif SIZE_X == 8 && SIZE_Y == 8
			return ( b >> 7 ) & 0x00fefefefefefefeULL;
#endif
		}
		Bitboard transferLeftTop(Bitboard b)
		{
#ifndef USE_SIMD
#
#	if SIZE_X == 4 && SIZE_Y == 4
			return ( b << 9 ) & 0xe0e0e00000000000ULL;
#	elif SIZE_X == 4 && SIZE_Y == 5
			return ( b << 9 ) & 0xe0e0e0e000000000ULL;
#	elif SIZE_X == 5 && SIZE_Y == 4
			return ( b << 9 ) & 0xf0f0f00000000000ULL;
#	elif SIZE_X == 5 && SIZE_Y == 5
			return ( b << 9 ) & 0xf0f0f0f000000000ULL;
#	elif SIZE_X == 5 && SIZE_Y == 6
			return ( b << 9 ) & 0xf0f0f0f0f0000000ULL;
#	elif SIZE_X == 6 && SIZE_Y == 5
			return ( b << 9 ) & 0xf8f8f8f800000000ULL;
#	elif SIZE_X == 6 && SIZE_Y == 6
			return ( b << 9 ) & 0xf8f8f8f8f8000000ULL;
#	elif SIZE_X == 6 && SIZE_Y == 7
			return ( b << 9 ) & 0xf8f8f8f8f8f80000ULL;
#	elif SIZE_X == 7 && SIZE_Y == 6
			return ( b << 9 ) & 0xfcfcfcfcfc000000ULL;
#	elif SIZE_X == 7 && SIZE_Y == 7
			return ( b << 9 ) & 0xfcfcfcfcfcfc0000ULL;
#	elif SIZE_X == 7 && SIZE_Y == 8
			return ( b << 9 ) & 0xfcfcfcfcfcfcfc00ULL;
#	elif SIZE_X == 8 && SIZE_Y == 7
			return ( b << 9 ) & 0xfefefefefefe0000ULL;
#	elif SIZE_X == 8 && SIZE_Y == 8
			return ( b << 9 ) & 0xfefefefefefefe00ULL;
#	endif

#else

// 5x5 (�x��)
			Bitboard mask = 0xf0f0f0f000000000ULL;
			_asm
			{
				movq	mm0, b
				psrlq	mm0, 9
				movq	mm1, mask
				pand	mm0, mm1
				movq	b, mm0
				emms
			}
			return b;

#endif

		}


		// MMX���߂��g���邩�ǂ���
		/*
		bool isMMXEnable()
		{
			int f = 0;
			
			_asm
			{
				push eax
				push ebx
				push ecx
				push edx

				mov eax,1
				cpuid
				mov f,edx

				pop edx
				pop ecx
				pop ebx
				pop eax
			}
			
			return ( ( f & 0x00800000 ) != 0 );
		}
        */

	}
}

