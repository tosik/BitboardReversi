/** @file
 * @brief	Bitboard Reversi �ݒ�t�@�C��
 *
 * @author	Toshiyuki Hirooka
 */

#pragma once


/** �{�[�h�̃T�C�YX */
#ifndef SIZE_X
//#			define	SIZE_X	4
//#			define	SIZE_X	5
#endif
/** �{�[�h�̃T�C�YY */
#ifndef SIZE_Y
//#			define	SIZE_Y	4
//#			define	SIZE_Y	5
#endif


/** �A���S���Y���̎�� */
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
/** �S�m�[�h�����J�E���g����i���[�g�m�[�h���܂ށj */
#			define COUNT_NODES_ALL
#endif
#ifndef COUNT_NODES_TERM
/** �I�[�m�[�h�����J�E���g����(�������I����Ă���m�[�h) */
//#			define COUNT_NODES_TERM
#endif
#ifndef COUNT_NODES_LIMITED
/** �r���ŉ�͂��I�������m�[�h�����J�E���g���� */
//#			define COUNT_NODES_LIMITED
#endif


/** �T����r���ŏI��������E��L���ɂ��邩
 * TODO �o�O�����݂���
 */
#ifndef ENABLE_LIMITED
//#		define ENABLE_LIMITED
#endif


/** �T������[��
 *
 * �œK�������߂�Ȃ�Ō�A�܂� SIZE_X*SIZE_Y-4 �𗘗p����
 * ���d�l�Ȃ̂ŐG��Ȃ��悤�ɁB
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


////// �f�o�b�O�p

/** �T�����̃{�[�h��\�����邩(�������Ԃ����Ȃ葝����) */
#ifndef VIEW_BOARD
//#			define VIEW_BOARD
#endif

/** moblist ��\�����邩 */
#ifndef VIEW_MOBLIST
//#		define VIEW_MOBLIST
#endif

/** �œK��̊�����\�����邩 */
#ifndef VIEW_KIFU
//#			define VIEW_KIFU
#endif

/** �œK��̊������{�[�h�ŕ\�����邩 */
#ifndef VIEW_KIFU_BOARD
//#			define VIEW_KIFU_BOARD
#endif

/** �����{�[�h��^���郂�[�h */
#ifndef INIT_BOARD
//#define INIT_BOARD
#endif

/** �͂��߂̈�肾�����o�͂��� */
#ifndef VIEW_FIRST_MOB
//#define VIEW_FIRST_MOB
#endif

/** �L���b�V�����[�h��L���ɂ���
 * TODO �o�O�����݂���
 */
#ifndef ENABLE_CACHE
//#define ENABLE_CACHE
#endif

/** �}����̎����L�^���� */
#ifndef RECORD_PRUNING_QUALITY
//#define RECORD_PRUNING_QUALITY
#endif

/** ���S�v���C�̑��x�Ȃǂ�CSV�`���ŏo�͂��� */
#ifndef VIEW_RESULT_BY_CSV
//#define VIEW_RESULT_BY_CSV
#endif


// �ݒ肪�����������؂���
#include "validator.h"


