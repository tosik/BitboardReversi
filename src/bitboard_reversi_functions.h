/** @file
 * @brief	Bitboard Reversi �֐��̃w�b�_�t�@�C��
 *
 * @author	Toshiyuki Hirooka
 */

#pragma once


#include <stack>

#include "setting.h"


/** ���c������ */
namespace Fujitalab
{
	/** ���o�[�V */
	namespace Reversi
	{

		// �{�[�h�̍ő�T�C�Y�iBitboard �͍ő� 64bits �̕ϐ���p���邽�߁A����ȏ�͕s�\�B�����I�Ȋg���̂��߂̒萔�j
		/** �{�[�h��X���ő�T�C�Y */
		const int MAX_SIZE_X = 8;
		/** �{�[�h��Y���ő�T�C�Y */
		const int MAX_SIZE_Y = 8;
		/** �{�[�h�̍ő�T�C�Y */
		const int MAX_SIZE = (MAX_SIZE_X*MAX_SIZE_Y);

		/** Bitboard�^�̒�`
         *
         * �e�r�b�g�͋ǖʂ�1�}�X�ɑΉ����Ă���B
         * 64bit���c8��8�̃r�b�g��ƍl����\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         * 00000000\n
         *
         * ����͗l�X�ȏ�ʂŎg���Ă���B���Ƃ��΋ǖʂ�\������ɂ͍��̃r�b�g�{�[�h�Ɣ��̃r�b�g�{�[�h�̓��p����B
         * ����\�ȍ��W�������ꍇ���r�b�g�{�[�h�ŕ\���ł���B�ڂ����͊e��`�ӏ����Q�Ƃ̂��ƁB
         */
		typedef unsigned long long int Bitboard;

		/** �ȈՔ�Board�^�̒�`�i�ǖʂ̏�ԁj*/
		typedef struct
		{
			/** ���΂̃r�b�g�{�[�h */
			Bitboard black;
			/** ���΂̃r�b�g�{�[�h */
			Bitboard white;
			/** ��ԁi���Ȃ�true, ���Ȃ�false�j */
			bool current_color;
		} SimpleBoard;

		/** Board�^�̒�`�i�ǖʂ̏�ԁj*/
		typedef struct
		{
			/** ���΂̃r�b�g�{�[�h */
			Bitboard black;
			/** ���΂̃r�b�g�{�[�h */
			Bitboard white;
			/** ��ԁi���Ȃ�true, ���Ȃ�false�j */
			bool current_color;
			/** ���Z�����������ɍs�����߂Ɏg���t���O�i������Ƃ��Ă܂��j */
			Bitboard left, right, top, bottom, lefttop, leftbottom, righttop, rightbottom;
		} Board;

		/** UNDO�Ȃǂɗp����{�[�h�̏�Ԃ�ۑ�����X�^�b�N */
		typedef std::stack<Board> BoardStack; // [TODO] �W�����C�u�������g�킸�A���͂Ŏ������č���������

		/** ��Q�̑ΐ����擾����
		 *
		 * @param b	�ΐ��ɕϊ����������l
		 * @return	�ΐ��ɕϊ����ꂽ�l*/
		int log2(Bitboard b);

		/** ���W�� Bitboard �ɂ���
		 *
		 * �^�������W�̃r�b�g��1,���̑���0�̃r�b�g�{�[�h���擾
		 * @param x	X���W�̒l
		 * @param y	Y���W�̒l
		 * @return	X,Y�����ɕϊ������r�b�g�{�[�h
		 */
		Bitboard toBitboard(int x, int y);

		/** �r�b�g��̂����͂��߂ɏo������r�b�g�������擾
		 *
         * ��F01101000 -> 00001000
		 * @param bits	�r�b�g��
		 * @return		�͂��߂ɏo������r�b�g������1�̃r�b�g�{�[�h
		 */
		Bitboard getFirstBit(Bitboard bits);

		/** �r�b�g��� 1 �̐����擾
		 *
		 * @param n	1�̐����J�E���g�������r�b�g��
		 * @return	�J�E���g����1�̐�
		 */
		int count(Bitboard n);

		// 
		/** �r�b�g�{�[�h��\������
		 *
		 * @param b	�\���������r�b�g�{�[�h
		 */
		void view(Bitboard b);

		/** ����\�Ȏ���擾(�������ɂЂ�����Ԃ���)
		 *
		 * @param black	�ǖʂ̍��̏��
		 * @param white	�ǖʂ̔��̏��
		 * @return		����\�ȍ��W��1�̃r�b�g�{�[�h
		 */
		Bitboard getMobilityLeft(Bitboard black, Bitboard white);
		/** ����\�Ȏ���擾(�E�����ɂЂ�����Ԃ���)
		 *
		 * @param black	�ǖʂ̍��̏��
		 * @param white	�ǖʂ̔��̏��
		 * @return		����\�ȍ��W��1�̃r�b�g�{�[�h
		 */
		Bitboard getMobilityRight(Bitboard black, Bitboard white);
		/** ����\�Ȏ���擾(������ɂЂ�����Ԃ���)
		 *
		 * @param black	�ǖʂ̍��̏��
		 * @param white	�ǖʂ̔��̏��
		 * @return		����\�ȍ��W��1�̃r�b�g�{�[�h
		 */
		Bitboard getMobilityTop(Bitboard black, Bitboard white);
		/** ����\�Ȏ���擾(�������ɂЂ�����Ԃ���)
		 *
		 * @param black	�ǖʂ̍��̏��
		 * @param white	�ǖʂ̔��̏��
		 * @return		����\�ȍ��W��1�̃r�b�g�{�[�h
		 */
		Bitboard getMobilityBottom(Bitboard black, Bitboard white);
		/** ����\�Ȏ���擾(��������ɂЂ�����Ԃ���)
		 *
		 * @param black	�ǖʂ̍��̏��
		 * @param white	�ǖʂ̔��̏��
		 * @return		����\�ȍ��W��1�̃r�b�g�{�[�h
		 */
		Bitboard getMobilityLeftTop(Bitboard black, Bitboard white);
		/** ����\�Ȏ���擾(���������ɂЂ�����Ԃ���)
		 *
		 * @param black	�ǖʂ̍��̏��
		 * @param white	�ǖʂ̔��̏��
		 * @return		����\�ȍ��W��1�̃r�b�g�{�[�h
		 */
		Bitboard getMobilityLeftBottom(Bitboard black, Bitboard white);
		/** ����\�Ȏ���擾(�E������ɂЂ�����Ԃ���)
		 *
		 * @param black	�ǖʂ̍��̏��
		 * @param white	�ǖʂ̔��̏��
		 * @return		����\�ȍ��W��1�̃r�b�g�{�[�h
		 */
		Bitboard getMobilityRightTop(Bitboard black, Bitboard white);
		/** ����\�Ȏ���擾(�E�������ɂЂ�����Ԃ���)
		 *
		 * @param black	�ǖʂ̍��̏��
		 * @param white	�ǖʂ̔��̏��
		 * @return		����\�ȍ��W��1�̃r�b�g�{�[�h
		 */
		Bitboard getMobilityRightBottom(Bitboard black, Bitboard white);

		/** �r�b�g�{�[�h��90�x�ɉ�]
		 *
		 * @param board	��]���������r�b�g�{�[�h
		 * @return 		��]���ꂽ�r�b�g�{�[�h
		 */
		Bitboard rotate(Bitboard board);

		/** �r�b�g�{�[�h����������ֈړ�����i�}�X�L���O���������˂�j*/
		Bitboard transferLeft(Bitboard b);
		Bitboard transferRight(Bitboard b);
		Bitboard transferTop(Bitboard b);
		Bitboard transferBottom(Bitboard b);
		Bitboard transferLeftTop(Bitboard b);
		Bitboard transferLeftBottom(Bitboard b);
		Bitboard transferRightTop(Bitboard b);
		Bitboard transferRightBottom(Bitboard b);

		// MMX ���g���邩
		bool isMMXEnable();

	}
}
