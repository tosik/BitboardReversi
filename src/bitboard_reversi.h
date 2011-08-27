/** @file
 * @brief	Bitboard Reversi �w�b�_�t�@�C��
 * @author	Toshiyuki Hirooka
 *
 * Bitboard ��p���� NxM �ɑΉ��̃��o�[�V
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

		/** Bitboard�ł̃��o�[�V�N���X
		 *
		 */
		class BitboardReversi
		{
		public:

			/** �̏��(����) */
			Bitboard black;
			/** �̏��(����) */
			Bitboard white;

			/** ���(true = ���A false = ��) */
			bool current_color;

			/** stack(undo�p) */
			BoardStack board_stack;

			/** ��サ�������Ď�����ϐ� */
			bool changed;

			/** �R���X�g���N�^(�I�u�W�F�N�g�̏�����) */
			BitboardReversi();

			/** �ݒu����i�Ђ�����Ԃ��Ȃ��j
			 *
			 * @param b	�ݒu������W�݂̂�1�̃r�b�g�{�[�h
			 * @param color	�ݒu����F
			 */
			void set(Bitboard b, bool color);

			/** ���肷��i�Ђ�����Ԃ��j
			 *
			 * �������p�ɍœK������Ă��邽�߁A����̏����ł̂ݐ���������
			 * @param m	���肷����W�̃r�b�g�{�[�h
			 */
			void put(Bitboard m);

			/** x,y ���g���Ē��肷��i�Ђ�����Ԃ��j
			 *
			 * @param x	���肷��X���W
			 * @param y	���肷��Y���W
			 */
			void put(int x, int y);

			/** ���肷��i�Ђ�����Ԃ��j���S��
			 *
			 * put���������p�ɍœK������Ă���̂ɑ΂��āA
			 * ������͒ᑬ������ԂɈˑ����邱�ƂȂ����ł��������������Ƃ�ۏႵ�Ă���i�܂���S�j
			 * @param m	���肷����W�̃r�b�g�{�[�h
			 */
			void putSafety(Bitboard m);

			/** x,y ���g���Ē��肷����S��
			 *
			 * @param x	���肷��X���W
			 * @param y	���肷��Y���W
			 */
			void putSafety(int x, int y);

			/** 1��߂�
			 *
			 */
			void undo();

			/** ���݂̋ǖʂ̏�Ԃ� Board �^�ɂ���
			 *
			 * @return	�ǖʂ̏��
			 */
			Board toBoard();

			/** �ǖʂ̏�Ԃ�^����
			 *
			 * @param black	���̏��
			 * @param white	���̏��
			 * @param current_color	���
			 */
			void setBoard(Bitboard black, Bitboard white, bool current_color);

			/** ���݂̋ǖʂ�\��
			 *
			 */
			void view();

			/** ���݂̋ǖʂ̒���\�ӏ���\��
			 *
			 */
			void viewMobility();

			/** ����\�ȉӏ����r�b�g�{�[�h�Ŏ擾
			 *
			 * @return	����\�ȉӏ��̃r�b�g�{�[�h
			 */
			Bitboard getMobility();

			/** �^�����ǖʂɂ����āA����\�ȉӏ����r�b�g�{�[�h�Ŏ擾
			 *
			 * @param black	���̏��
			 * @param white	���̏��
			 * @return	����\�ȉӏ��̃r�b�g�{�[�h
			 */
			Bitboard getMobility(Bitboard black, Bitboard white);

			/** ����\���������邽�߂̓���ȕϐ�
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

			/** �����ɔ��]����΂�������Ă���r�b�g�{�[�h���擾
			 *
			 * @param mob	���肷��r�b�g�{�[�h
			 * @return	���]����΂̃p�^�[���̃r�b�g�{�[�h
			 */
			Bitboard getReversedPattern(Bitboard mob);

		};
	}
}
