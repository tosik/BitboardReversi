/** @file
 * @brief	Move Ordering�]���֐�
 *
 * @author	Toshiyuki Hirooka
 *
 */

#include "bitboard_reversi.h"
using namespace Fujitalab::Reversi;


/** �J���x���擾
 *
 * @param	mobility	����\�ꏊ�̃r�b�g�{�[�h
 * @param	o	�T�����̃��o�[�V�̃I�u�W�F�N�g�ւ̃|�C���^
 * @param	depth	�T������[��
 * @return	�J���x
 */
int getOpenValue(Bitboard mobility, BitboardReversi *o, int depth);

/** �J���x���_�Ɋ�Â��ėD�G�ȃr�b�g���擾
 *
 * [TODO] Potential Mobility �Ƃ������O�Œm���Ă���l�q
 * �ڍׂ� potential mobility reversi ����Google����
 *
 * @param	mobility	����\�ꏊ�̃r�b�g�{�[�h
 * @param	o	�T�����̃��o�[�V�̃I�u�W�F�N�g�ւ̃|�C���^
 * @param	depth	�T������[���i2�ȏ�Ŕ��W�I�J���x���_�j
 * @return	�ł��D�G�Ȓ���\�ȏꏊ�̃r�b�g������1�̃r�b�g�{�[�h
 */
Bitboard getBitByOpenTheory(Bitboard mobility, BitboardReversi *o, int depth);

/** �l�K�A���t�@�ɂ��]���l���擾����
 *
 * @param	mobility	����\�ꏊ�̃r�b�g�{�[�h
 * @param	o	�T�����̃��o�[�V�̃I�u�W�F�N�g�ւ̃|�C���^
 * @param	depth	�T������[��
 * @param	alpha	���J�b�g�p�ϐ�
 * @param	beta	���J�b�g�p�ϐ�
 * @return	�ł��D�G�Ȓ���\�ȏꏊ�̃r�b�g������1�̃r�b�g�{�[�h
 */
int getValueByNegaalpha(Bitboard mobility, BitboardReversi *o, int depth, int alpha, int beta);

/** �l�K�A���t�@��p���čł��D�G�Ȏ���擾����
 *
 * @param	mobility	����\�ꏊ�̃r�b�g�{�[�h
 * @param	o	�T�����̃��o�[�V�̃I�u�W�F�N�g�ւ̃|�C���^
 * @param	depth	�T������[��
 * @return	�ł��D�G�Ȓ���\�ȏꏊ�̃r�b�g������1�̃r�b�g�{�[�h
 */
Bitboard getBitByNegaalpha(Bitboard mobility, BitboardReversi *o, int depth);

/** ���Ғl�@�Ɋ�Â��Ċ��Ғl���擾����
 *
 * @param	mobility	����\�ꏊ�̃r�b�g�{�[�h
 * @param	o	�T�����̃��o�[�V�̃I�u�W�F�N�g�ւ̃|�C���^
 * @param	depth	�T������[��
 * @param	alpha	���J�b�g�p�ϐ�
 * @param	beta	���J�b�g�p�ϐ�
 * @return	�ł��D�G�Ȓ���\�ȏꏊ�̃r�b�g������1�̃r�b�g�{�[�h
 */
float getExpectation(Bitboard mobility, BitboardReversi *o, int depth, float alpha, float beta);

/** ���Ғl�@�Ɋ�Â��čł��D�G�Ȏ���擾����
 * ���Ғl�@�� http://ci.nii.ac.jp/naid/110002771150/ ��p���Ă���
 *
 * @param	mobility	����\�ꏊ�̃r�b�g�{�[�h
 * @param	o	�T�����̃��o�[�V�̃I�u�W�F�N�g�ւ̃|�C���^
 * @param	depth	�T������[��
 * @return	�ł��D�G�Ȓ���\�ȏꏊ�̃r�b�g������1�̃r�b�g�{�[�h
 */
Bitboard getBitByExpectationMethod(Bitboard mobility, BitboardReversi *o, int depth);

/** N���̒���\�Ȏ萔���ŏ��̂��̂��擾
 *
 * @param	mobility	����\�ꏊ�̃r�b�g�{�[�h
 * @param	o	�T�����̃��o�[�V�̃I�u�W�F�N�g�ւ̃|�C���^
 * @param	depth	�T������[��
 * @return	�ł��D�G�Ȓ���\�ȏꏊ�̃r�b�g������1�̃r�b�g�{�[�h
 */
int getMNM(Bitboard mobility, BitboardReversi *o, int depth);

/** N���̒���\�Ȏ萔���ŏ��ɂȂ����擾
 * �����؂����������p�X������肪�D��I�ɑI�������
 *
 * MNM�@ (Minimum Number of Mobilities Method)
 *
 * @param	mobility	����\�ꏊ�̃r�b�g�{�[�h
 * @param	o	�T�����̃��o�[�V�̃I�u�W�F�N�g�ւ̃|�C���^
 * @param	depth	�T������[��
 * @return	�ł��D�G�Ȓ���\�ȏꏊ�̃r�b�g������1�̃r�b�g�{�[�h
 */
Bitboard getBitByMNM(Bitboard mobility, BitboardReversi *o, int depth);

/** �����_���Ŏ������
 *
 * @param	mobility	����\�ꏊ�̃r�b�g�{�[�h
 * @return	�����_���őI�����ꂽ����\�ȏꏊ�̃r�b�g������1�̃r�b�g�{�[�h
 */
Bitboard getBitByRandom(Bitboard mobility);

/** �r�b�g��̃e�[�u�����Q�Ƃ��A���̒��ōł��D�G�ȃr�b�g�������擾
 *
 * @param bits	�r�b�g��
 * @return		�D�G�ȃr�b�g������1�̃r�b�g�{�[�h
 */
Bitboard getBitByTable(Bitboard bits);

